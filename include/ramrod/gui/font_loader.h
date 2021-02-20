#ifndef RAMROD_GUI_FONT_LOADER_H
#define RAMROD_GUI_FONT_LOADER_H

#include <string>
#include <map>

#include "ramrod/gl/texture.h"
#include "ramrod/gui/types.h"

namespace ramrod {
  namespace gui {
    class font_loader
    {
    public:
      /**
       * @brief Constructor
       * @param font_distance_path Path towards the image file that contains
       *                           the font's distance field
       * @param font_info_path Path towards the font's info file with extension .fnt
       */
      font_loader(const std::string &font_distance_path,
                  const std::string &font_info_path);
      /**
       * @brief Getting all the loaded characters from this font
       * @return map with the ascii code as KEY and the character information as VALUE
       */
      const std::map<const std::string, gui::text::character> &characters();
      /**
       * @brief Getting the font name that is obtained from the info file (.fnt)
       * @return String containing the real font name
       */
      const std::string &font_name();
      /**
       * @brief Activates and bind the font texture into openGL
       * @return `false` if there was an error loading the font
       */
      bool use();
      /**
       * @brief Converting a decimal unicode into its hexadecimal value
       * @param number Decimal unicode value
       * @return A string containing the hexadecimal letters
       */
      static std::string decimal_to_unicode(uint32_t number);
      /**
       * @brief Converting from decimal unicode value into a utf-8 character
       * @param code Decimal unicode value
       * @return A utf-8 string that could have a size bigger than 1 char but enough to
       *         contain only one Unicode character
       */
      static std::string unicode_to_utf8(uint32_t code);

    private:
      void load_distance();
      void load_info();

      std::string font_name_;
      std::string distance_path_, info_path_;
      bool file_exists_, is_loaded_;

      ramrod::gl::texture texture_;
      std::map<const std::string, gui::text::character> characters_;

      bool error_;
    };
  } // namespace: gui
} // namespace: ramrod

#endif // RAMROD_GUI_FONT_LOADER_H
