#ifndef RAMROD_GUI_TEXTURE_H
#define RAMROD_GUI_TEXTURE_H

#include <string>

#include "ramrod/gl/texture.h"

namespace ramrod {
  namespace gui {
    class window;

    class texture : public gl::texture
    {
    public:
      /**
       * @brief Creation of a texture object
       *
       * @param image_path      Absolute file's path
       * @param generate_mipmap Indicates if the texture shoudl have mipmaps
       */
      texture(gui::window *window, const std::string &image_path,
              const bool generate_mipmap = true);
      int height();
      /**
       * @brief Loading a new image for this texture which replaces old data
       *
       * @param image_path File path for the replacing image
       *
       * @return `false` if image was not found or there is an error with is format
       */
      bool load(const std::string &image_path);
      /**
       * @brief Obtaining the image's path
       *
       * @return Image's absolute path
       */
      const std::string &path();
      int number_of_components();
      /**
       * @brief Reloads the image's data
       *
       * @return `false` if image was not found or there is an error with is format
       */
      bool reload();
      int width();

    protected:

    private:
      std::string texture_path_;
      int width_;
      int height_;
      int number_of_components_;

    };
  } // namespace: gui
} // namespace: ramrod

#endif // RAMROD_GUI_TEXTURE_H
