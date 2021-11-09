#ifndef APPS_GLOBAL_PREFERENCES_H
#define APPS_GLOBAL_PREFERENCES_H

#include <apps/i18n.h>
#include <kandinsky/font.h>
#include <ion.h>

class GlobalPreferences {
public:
  enum class ExamMode : int8_t {
    Unknown = -1,
    Off = 0,
    Standard = 1,
    Dutch = 2,
    PressToTest = 3
  };
  /* By default, a 0 PressToTestParams has all parameters set to false. Params
   * are false if the feature is activated (allowed) and true if forbidden. */
  typedef union {
    uint8_t m_value;
    struct {
      bool m_isUnknown: 1;
      bool m_equationSolverIsForbidden: 1;
      bool m_inequalityGraphingIsForbidden: 1;
      bool m_implicitPlotsAreForbidden: 1;
      bool m_statsDiagnosticsAreForbidden: 1;
      bool m_vectorsAreForbidden: 1;
      bool m_basedLogarithmIsForbidden: 1;
      bool m_sumIsForbidden: 1;
    };
  } PressToTestParams;
  static_assert(sizeof(PressToTestParams) == sizeof(uint8_t), "PressToTestParams can have 8 params at most");

  static GlobalPreferences * sharedGlobalPreferences();
  I18n::Language language() const { return m_language; }
  void setLanguage(I18n::Language language) { m_language = language; }
  I18n::Country country() const { return m_country; }
  void setCountry(I18n::Country country) { m_country = country; }
  CountryPreferences::AvailableExamModes availableExamModes() const { return I18n::CountryPreferencesArray[static_cast<uint8_t>(m_country)].availableExamModes(); }
  CountryPreferences::MethodForQuartiles methodForQuartiles() const { return I18n::CountryPreferencesArray[static_cast<uint8_t>(m_country)].methodForQuartiles(); }
  Poincare::Preferences::UnitFormat unitFormat() const { return I18n::CountryPreferencesArray[static_cast<uint8_t>(m_country)].unitFormat(); }
  CountryPreferences::HomeAppsLayout homeAppsLayout() const { return I18n::CountryPreferencesArray[static_cast<uint8_t>(m_country)].homeAppsLayout(); }
  const char * discriminantSymbol() const { return I18n::CountryPreferencesArray[static_cast<uint8_t>(m_country)].discriminantSymbol(); }
  bool isInExamMode() const { return (int8_t)examMode() > 0; }
  ExamMode examMode() const;
  PressToTestParams pressToTestParams() const;
  void setExamMode(ExamMode examMode);
  void setPressToTestParams(PressToTestParams pressToTestParams);
  static_assert((int8_t)GlobalPreferences::ExamMode::Off == 0, "GlobalPreferences::isInExamMode() is not right");
  static_assert((int8_t)GlobalPreferences::ExamMode::Unknown < 0, "GlobalPreferences::isInExamMode() is not right");

  bool showPopUp() const { return m_showPopUp; }
  void setShowPopUp(bool showPopUp) { m_showPopUp = showPopUp; }

  constexpr static int NumberOfBrightnessStates = 12;
  int brightnessLevel() const { return m_brightnessLevel; }
  void setBrightnessLevel(int brightnessLevel);

  const KDFont * font() const { return m_font; }
  void setFont(const KDFont * font) { m_font = font; }

  // Check if both exam mode and press-to-test parameters forbid these features
  bool equationSolverIsForbidden() const;
  bool inequalityGraphingIsForbidden() const;
  bool implicitPlotsAreForbidden() const;
  bool statsDiagnosticsAreForbidden() const;
  bool vectorsAreForbidden() const;
  bool basedLogarithmIsForbidden() const;
  bool sumIsForbidden() const;

private:
  static constexpr int k_examModePersistingByteIndex = 0;
  static constexpr int k_pressToTestParamsPersistingByteIndex = 1;
  static_assert(I18n::NumberOfLanguages > 0, "I18n::NumberOfLanguages is not superior to 0"); // There should already have been an error when processing an empty EPSILON_I18N flag
  static_assert(I18n::NumberOfCountries > 0, "I18n::NumberOfCountries is not superior to 0"); // There should already have been an error when processing an empty EPSILON_COUNTRIES flag
  GlobalPreferences() :
    m_language((I18n::Language)0),
    m_country((I18n::Country)0),
    m_examMode(ExamMode::Unknown),
    m_showPopUp(true),
    m_brightnessLevel(Ion::Backlight::MaxBrightness),
    m_font(KDFont::LargeFont) { m_pressToTestParams.m_isUnknown = true; }
  /* Some ExamMode Press-to-test parameters must be replicated in
   * Poincare::Preferences::sharedPreferences() */
  void updatePoincareSharedPreferences() const;
  I18n::Language m_language;
  I18n::Country m_country;
  static_assert((int8_t)GlobalPreferences::ExamMode::Off == 0, "GlobalPreferences::isInExamMode() is not right");
  static_assert((int8_t)GlobalPreferences::ExamMode::Unknown < 0, "GlobalPreferences::isInExamMode() is not right");
  mutable ExamMode m_examMode;
  mutable PressToTestParams m_pressToTestParams;
  bool m_showPopUp;
  int m_brightnessLevel;
  const KDFont * m_font;
};

#endif
