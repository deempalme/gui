#include "ramrod/gui/image_loader.h"

#define STBI_FAILURE_USERMSG
#include "stb_image.h"

#include "ramrod/gui/file_manager.h"

namespace ramrod::gui {
  image_loader::image_loader() :
    image_path_(),
    image_data_(nullptr),
    width_{0},
    height_{0},
    components_{0},
    desired_channels_{0},
    loaded_{false}
  {}

  image_loader::image_loader(const std::string &path, const bool invert_y_axis,
                             const int desired_channels) :
    image_path_(path),
    image_data_(nullptr),
    width_{0},
    height_{0},
    components_{0},
    desired_channels_{desired_channels},
    loaded_{false}
  {
    load(path, invert_y_axis, desired_channels);
  }

  image_loader::~image_loader(){
    if(image_data_ != nullptr) stbi_image_free(image_data_);
  }

  int image_loader::components(){
    return components_;
  }

  unsigned char *image_loader::data(){
    return image_data_;
  }

  const char *image_loader::error(){
    return stbi_failure_reason();
  }

  void image_loader::free(){
    if(image_data_ != nullptr) stbi_image_free(image_data_);
    image_data_ = nullptr;
    loaded_ = false;
    width_ = height_ = components_ = 0;
    image_path_.clear();
  }

  int image_loader::height(){
    return height_;
  }

  bool image_loader::load(const std::string &path, const bool invert_y_axis,
                          const int desired_channels){
    if(image_data_ != nullptr) free();

    if(gui::file_manager::check_path(image_path_)){
      stbi_set_flip_vertically_on_load(invert_y_axis ? 1 : 0);
      image_data_ = stbi_load(image_path_.c_str(), &width_, &height_,
                              &components_, desired_channels_);

      if(image_data_ != nullptr)
        return loaded_ = true;
    }
    return false;
  }

  bool image_loader::loaded(){
    return loaded_;
  }

  const std::string &image_loader::path(){
    return image_path_;
  }

  int image_loader::width(){
    return width_;
  }
} // namespace: ramrod::gui
