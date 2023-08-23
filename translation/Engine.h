// ======================================================================

#pragma once

#include <spine/SmartMetEngine.h>
#include <spine/Translations.h>
#include <boost/shared_ptr.hpp>


namespace SmartMet
{
namespace Engine
{
namespace Translation
{

class Engine : public SmartMet::Spine::SmartMetEngine
{
 public:
  // constructor is available only with a libconfig configuration file

  Engine() = delete;
  explicit Engine(std::string theFileName);

  boost::optional<std::string> getParameterTranslation(const std::string& theParam,
													   int theValue,
													   const std::string& theLanguage) const;
  boost::optional<std::string> getStringTranslation(const std::string& theKey,
													const std::string& theLanguage) const;
  boost::optional<std::vector<std::string>> getStringArrayTranslation(const std::string& theKey,
																	  const std::string& theLanguage) const;

 protected:
  void init() override;
  void shutdown() override;

 private:

  std::string itsConfigFile;
  boost::shared_ptr<Spine::Translations> itsTranslations;


};  // class Engine

}  // namespace Translation
}  // namespace Engine
}  // namespace SmartMet

// ======================================================================
