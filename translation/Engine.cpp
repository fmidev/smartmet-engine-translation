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

    itsTranslations = boost::make_shared<Spine::Translations>(config);	
  }
  catch (...)
  {
    throw Fmi::Exception::Trace(BCP, "Operation failed!");
  }
}

boost::optional<std::string> Engine::getParameterTranslation(const std::string& theParam,
															 int theValue,
															 const std::string& theLanguage) const
{  
  return itsTranslations->getParameterTranslation(theParam, theValue, theLanguage);
}

boost::optional<std::string> Engine::getStringTranslation(const std::string& theKey,
															const std::string& theLanguage) const
{
  return itsTranslations->getStringTranslation(theKey, theLanguage);
}

boost::optional<std::vector<std::string>> Engine::getStringArrayTranslation(const std::string& theKey,
																			const std::string& theLanguage) const
{
  return itsTranslations->getStringArrayTranslation(theKey, theLanguage);
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
