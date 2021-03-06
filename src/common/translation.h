/*
   mkvmerge -- utility for splicing together matroska files
   from component media subtypes

   Distributed under the GPL v2
   see the file COPYING for details
   or visit http://www.gnu.org/copyleft/gpl.html

   translation, locale handling

   Written by Moritz Bunkus <moritz@bunkus.org>.
*/

#ifndef MTX_COMMON_TRANSLATION_H
#define MTX_COMMON_TRANSLATION_H

#include "common/common_pch.h"

#include <ostream>

class translation_c {
public:
  static std::vector<translation_c> ms_available_translations;
  static int ms_active_translation_idx;

public:
  std::string m_iso639_2_code, m_unix_locale, m_windows_locale, m_windows_locale_sysname, m_english_name, m_translated_name;
  bool m_line_breaks_anywhere;
  int m_language_id, m_sub_language_id;

  translation_c(std::string const &iso639_2_code,
                std::string const &unix_locale,
                std::string const &windows_locale,
                std::string const &windows_locale_sysname,
                std::string const &english_name,
                std::string const &translated_name,
                bool line_breaks_anywhere,
                int language_id,
                int sub_language_id);

  std::string get_locale() const;
  bool matches(std::string const &locale) const;

  static void initialize_available_translations();
  static int look_up_translation(const std::string &locale);
  static int look_up_translation(int language_id, int sub_language_id);
  static std::string get_default_ui_locale();
  static translation_c &get_active_translation();
  static void set_active_translation(const std::string &locale);
};

class translatable_string_c {
protected:
  std::string m_untranslated_string;
  boost::optional<std::string> m_overridden_by;

public:
  translatable_string_c();
  translatable_string_c(const std::string &untranslated_string);
  translatable_string_c(const char *untranslated_string);

  std::string get_translated() const;
  std::string get_untranslated() const;

  void override(std::string const &by);
};

#define YT(s) translatable_string_c(s)

inline std::ostream &
operator <<(std::ostream &out,
            translatable_string_c const &s) {
  out << s.get_translated();
  return out;
}

void init_locales(std::string locale = "");

#endif  // MTX_COMMON_TRANSLATION_H
