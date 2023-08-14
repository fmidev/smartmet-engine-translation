// ======================================================================

#pragma once

#include <spine/SmartMetEngine.h>
#include <boost/shared_ptr.hpp>
#include "ParameterTranslations.h"


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

  boost::optional<std::string> getTranslation(const std::string& theParam,
                                              int theValue,
                                              const std::string& theLanguage) const;  
 protected:
  void init() override;
  void shutdown() override;

 private:

  std::string itsConfigFile;
  boost::shared_ptr<ParameterTranslations> itsParameterTranslations;


};  // class Engine

}  // namespace Translation
}  // namespace Engine
}  // namespace SmartMet

// ======================================================================
