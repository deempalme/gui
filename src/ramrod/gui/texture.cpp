#include "ramrod/gui/texture.h"

#include "ramrod/gui/gui_manager.h"
#include "ramrod/gui/image_loader.h"
#include "ramrod/gui/window.h"

namespace ramrod {
  namespace gui {
    ramrod::gui::texture::texture(gui::window *window, const std::string &image_path,
                                  const bool generate_mipmap) :
      gl::texture(true, window->last_texture_id(), generate_mipmap),
      texture_path_(),
      width_{0},
      height_{0},
      number_of_components_{0}
    {
      load(image_path);
    }

    int texture::height(){
      return height_;
    }

    bool texture::load(const std::string &image_path){
      texture_path_ = image_path;
      gui::image_loader image(image_path);
      if(image.loaded()){
        allocate(image.width(), image.height(), image.data(), image.channels_in_file());

        width_ = image.width();
        height_ = image.height();
        number_of_components_ = image.channels_in_file();

        return true;
      }

      width_ = height_ = number_of_components_ = 0;
      return false;
    }

    const std::string &texture::path(){
      return texture_path_;
    }

    int texture::number_of_components(){
      return number_of_components_;
    }

    bool texture::reload(){
      return load(texture_path_);
    }

    int texture::width(){
      return width_;
    }
  } // namespace: gui
} // namespace: ramrod
