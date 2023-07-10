#include "ramrod/gui/image_loader.h"

#define STBI_FAILURE_USERMSG
#include "stb_image.h"


namespace ramrod {
  namespace gui {
    image_loader::image_loader(const std::string &path,
                               const bool invert_y_axis,
                               const int desired_channels) :
      image_path_(path),
      image_data_(nullptr),
      width_{0},
      height_{0},
      channels_{0},
      loaded_{false}
    {
      load(path, invert_y_axis, desired_channels);
    }

    image_loader::~image_loader(){
      free();
    }

    int image_loader::channels_in_file(){
      return channels_;
    }

    unsigned char *image_loader::data(){
      return image_data_;
    }

    const char *image_loader::failure_reason(){
      return stbi_failure_reason();
    }

    void image_loader::free(){
      if(image_data_ != nullptr) stbi_image_free(image_data_);
      image_data_ = nullptr;
    }

    int image_loader::height(){
      return height_;
    }

    unsigned char *image_loader::load(const std::string &path,
                                      const bool invert_y_axis,
                                      const int desired_channels){
      if(image_data_ != nullptr) free();

      image_path_ = path;

      stbi_set_flip_vertically_on_load(invert_y_axis ? 1 : 0);
      image_data_ = stbi_load(image_path_.c_str(), &width_, &height_,
                              &channels_, desired_channels);

      loaded_ = image_data_ != nullptr;
      return image_data_;
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
  } // namespace gui
} // namespace ramrod
