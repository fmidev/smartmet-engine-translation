#include "ParameterTranslations.h"
#include <iostream>

namespace SmartMet
{
namespace Engine
{
namespace Translation
{
void ParameterTranslations::setDefaultLanguage(const std::string& theLanguage)
{
  itsDefaultLanguage = theLanguage;
}

const std::string& ParameterTranslations::getDefaultLanguage() const
{
  return itsDefaultLanguage;
}

void ParameterTranslations::addTranslation(const std::string& theParam,
                                           int theValue,
                                           const std::string& theLanguage,
                                           const std::string& theTranslation)
{
  itsTranslations[theParam][theValue][theLanguage] = theTranslation;
}

boost::optional<std::string> ParameterTranslations::getTranslation(
    const std::string& theParam, int theValue, const std::string& theLanguage) const
{
  auto param = itsTranslations.find(theParam);
  if (param == itsTranslations.end())
    return {};

  auto lang = param->second.find(theValue);
  if (lang == param->second.end())
    return {};

  auto trans = lang->second.find(theLanguage);
  if (trans != lang->second.end())
    return trans->second;

  // No translation found for the requested language, use default language instead
  trans = lang->second.find(itsDefaultLanguage);
  if (trans == lang->second.end())
    return {};

  return trans->second;
}

}  // namespace Translation
}  // namespace Engine
}  // namespace SmartMet
