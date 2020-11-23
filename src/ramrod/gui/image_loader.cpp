#include "ramrod/gui/image_loader.h"

#define STBI_FAILURE_USERMSG
#include "stb_image.h"

#include "ramrod/console.h"
#include "ramrod/gui/file_manager.h"

namespace ramrod {
  namespace gui {
    image_loader::image_loader(const std::string &path, const bool invert_y_axis,
                               const int desired_channels) :
      image_path_(path),
      image_data_(nullptr),
      width_{0},
      height_{0},
      components_{0},
      desired_channels_{desired_channels}
    {
      if(gui::file_manager::check_path(image_path_)){
        stbi_set_flip_vertically_on_load(invert_y_axis ? 1 : 0);
        image_data_ = stbi_load(image_path_.c_str(), &width_, &height_,
                                &components_, desired_channels_);

        if(image_data_ == nullptr)
          rr::error("There was an error when parsing image data");
      }else
        rr::error("Image not found.");
    }

    image_loader::~image_loader(){
      if(image_data_ != nullptr) stbi_image_free(image_data_);
    }

    unsigned char *image_loader::data(){
      return image_data_;
    }

    std::string image_loader::failure_reason(){
      return std::string(stbi_failure_reason());
    }

    GLenum image_loader::format(){
      switch(components_){
        case 1:
          return GL_RED;
        break;
        case 2:
          return GL_RG;
        break;
        case 3:
          return GL_RGB;
        break;
        case 4:
          return GL_RGBA;
        break;
      }
      return GL_NONE;
    }

    int image_loader::height(){
      return height_;
    }

    GLenum image_loader::internal_format(){
      switch(components_){
        case 1:
          return GL_R8;
        break;
        case 2:
          return GL_RG8;
        break;
        case 3:
          return GL_RGB8;
        break;
        case 4:
          return GL_RGBA8;
        break;
      }
      return GL_NONE;
    }

    int image_loader::number_of_components(){
      return components_;
    }

    const std::string &image_loader::path(){
      return image_path_;
    }

    int image_loader::width(){
      return width_;
    }
  } // namespace: gui
} // namespace: ramrod
