#include "Engine.h"
#include <boost/filesystem.hpp>
#include <json/reader.h>
#include <spine/ConfigTools.h>
#include <spine/Exceptions.h>
#include <macgyver/StringConversion.h>
#include <macgyver/Exception.h>


namespace SmartMet
{
namespace Engine
{
namespace Translation
{
namespace
{
ParameterTranslations read_translations(const libconfig::Config& config)
{
  try
  {
    Json::Reader jsonreader;

    ParameterTranslations translations;

    // Establish default language

    std::string language;
    if (config.lookupValue("language", language))
      translations.setDefaultLanguage(language);

    // Read all parameter translations. We assume JSON encoded strings to avoid config file
    // encoding ambiguities. libconfig itself provides no extra Unicode support.

    if (!config.exists("translations"))
      return translations;

    const libconfig::Setting& settings = config.lookup("translations");

    if (!settings.isGroup())
      throw Fmi::Exception(
          BCP, "translations must be a group of parameter name to translations mappings");

    for (int i = 0; i < settings.getLength(); i++)
    {
      const auto& param_settings = settings[i];
      if (!param_settings.isList())
        throw Fmi::Exception(BCP,
                             "translations must be lists of groups consisting of parameter value "
                             "and its translations");

      std::string param_name = Fmi::ascii_tolower_copy(param_settings.getName());
	  

      for (int j = 0; j < param_settings.getLength(); j++)
      {
        const auto& value_translations = param_settings[j];

        if (value_translations.isList())
          throw Fmi::Exception(BCP,
                               "translations for parameter " + param_name +
                                   " must be a list of translations for individual values");

        int param_value;
        if (!value_translations.lookupValue("value", param_value))
          throw Fmi::Exception(BCP,
                               "translation setting for " + param_name + " at position " +
                                   std::to_string(j) + " has no parameter value to be translated");

        for (int k = 0; k < value_translations.getLength(); k++)
        {
          const auto& translation = value_translations[k];

          std::string lang = translation.getName();
          if (lang == "value")
            continue;

          auto text = std::string("\"") + translation.c_str() + "\"";
          Json::Value json;
          bool ok = jsonreader.parse(text, json);
          if (!ok || !json.isString())
            throw Fmi::Exception(BCP, "Failed to parse JSON string '" + text + "'");

          translations.addTranslation(param_name, param_value, lang, json.asString());
        }
      }
    }

    return translations;
  }
  catch (...)
  {
    Spine::Exceptions::handle("Translation engine");
    throw;  // NOT REACHED, but silences compiler warning
  }
}
}  // namespace


// ----------------------------------------------------------------------
/*!
 * \brief The only permitted constructor requires a configfile
 */
// ----------------------------------------------------------------------

Engine::Engine(std::string theFileName) : itsConfigFile(std::move(theFileName)) 
{

}

// ----------------------------------------------------------------------
/*!
 * \brief Initialize the engine
 *
 * Note: We do not wish to delay the initialization of other engines.
 * init() is done in its own thread, hence we read the configuration
 * files here, and hence itsConfig is a pointer instead of an object.
 */
// ----------------------------------------------------------------------

void Engine::init()
{
  try
  {
    libconfig::Config config;

    // Enable sensible relative include paths
    boost::filesystem::path p = itsConfigFile;
    p.remove_filename();
    config.setIncludeDir(p.c_str());
    config.readFile(itsConfigFile.c_str());
    Spine::expandVariables(config);

    itsParameterTranslations = boost::make_shared<ParameterTranslations>(read_translations(config));
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}

boost::optional<std::string> Engine::getTranslation(const std::string& theParam,
													int theValue,
													const std::string& theLanguage) const
{  
  return itsParameterTranslations->getTranslation(theParam, theValue, theLanguage);
}

// ----------------------------------------------------------------------
/*!
 * \brief Shutdown the engine
 */
// ----------------------------------------------------------------------

void Engine::shutdown()
{
  std::cout << "  -- Shutdown requested (translation)\n";
}


}  // namespace Translation
}  // namespace Engine
}  // namespace SmartMet

// DYNAMIC MODULE CREATION TOOLS

extern "C" void* engine_class_creator(const char* configfile, void* /* user_data */)
{
  return new SmartMet::Engine::Translation::Engine(configfile);
}

extern "C" const char* engine_name()
{
  return "Translation";
}
// ======================================================================
