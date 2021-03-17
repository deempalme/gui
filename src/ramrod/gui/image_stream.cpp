#include "ramrod/gui/image_stream.h"

#include <cstring>

#include "ramrod/gl/pixel_buffer.h"
#include "ramrod/gl/texture.h"
#include "ramrod/gui/constants.h"
#include "ramrod/gui/window.h"

namespace ramrod {
  namespace gui {
    image_stream::image_stream(gui::window *window, const float width, const float height,
                               const int number_of_channels, const float position_x,
                               const float position_y, const int z_index,
                               const std::size_t tab_index) :
      gui::element(window, width, height, position_x, position_y, z_index, tab_index),
      pbo_{static_cast<gl::pixel_buffer*>(std::malloc(sizeof(gl::pixel_buffer) * 2))},
      read_pbo_{0},
      texture_{new gl::texture(false, gui::texture_unit::sprite, false)},
      width_{static_cast<int>(std::floor(width))},
      height_{static_cast<int>(std::floor(height))},
      resolution_{width_ * height_},
      data_format_{GL_RGB},
      internal_format_{GL_RGB8},
      channels_{3},
      pointer_{nullptr},
      mapped_{false}
    {
      switch(number_of_channels){
        case 4:
          // TODO: change GL_BGRA to GL_RGBA
          data_format_ = GL_BGRA;
          internal_format_ = GL_RGBA8;
          channels_ = 4;
        break;
        case 2:
          data_format_ = GL_RG;
          internal_format_ = GL_RG8;
          channels_ = 2;
        break;
        case 1:
          data_format_ = GL_RED;
          internal_format_ = GL_R8;
          channels_ = 1;
        break;
        default: break;
      }

      texture_->generate();
      texture_->bind();
      texture_->parameter(GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);
      texture_->allocate(width_, height_, nullptr, data_format_,
                         GL_UNSIGNED_BYTE, internal_format_);
      texture_->release();

      pbo_[0] = gl::pixel_buffer(false, gl::pixel::buffer::unpack);
      pbo_[0].generate();
      pbo_[0].bind();
      pbo_[0].allocate_data(nullptr, resolution_, GL_STREAM_DRAW);
      pbo_[0].release();

      pbo_[1] = gl::pixel_buffer(false, gl::pixel::buffer::unpack);
      pbo_[1].generate();
      pbo_[1].bind();
      pbo_[1].allocate_data(nullptr, resolution_, GL_STREAM_DRAW);
      pbo_[1].release();

      texture_coordinates(0.0f, 1.0f, 1.0f, 0.0f);

      element::texture_id(texture_->id());
    }

    image_stream::~image_stream(){
      std::free(pbo_);
      if(texture_) delete texture_;
    }

    char *image_stream::map(const int width, const int height){
      if(mapped_) unmap();

      // Frames get written after dequeuing the buffer
      read_pbo_ = (read_pbo_ + 1) % 2;
      const std::size_t write_pbo = (read_pbo_ + 1) % 2;

      pbo_[read_pbo_].bind();
      texture_->bind();
      if(width > 0 && height > 0 && (width != width_ || height != height_)){
        width_ = width;
        height_ = height;
        resolution_ = width * height * channels_;
        texture_->allocate(width_, height_, nullptr, data_format_,
                           GL_UNSIGNED_BYTE, internal_format_);
      }else
        texture_->allocate_sub_2d(width_, height_);

      mapped_ = true;
      pbo_[write_pbo].bind();
      pbo_[write_pbo].allocate_data(nullptr, resolution_, GL_STREAM_DRAW);
      return pointer_ = (char*)pbo_[write_pbo].map(GL_WRITE_ONLY);
    }

    void image_stream::paint(){
      if(!display_) return;

      window_->bind_shader(shader_id_);
      window_->bind_texture(texture_id_, texture_->active_texture_id());

      buffer_.vertex_bind();
      buffer_.draw(GL_POINTS, 0, 1);
      buffer_.vertex_release();
    }

    void image_stream::texture_id(const GLuint /*new_texture_id*/){}

    bool image_stream::texture_size(const int width, const int height){
      if(width <= 0 || height <= 0) return false;

      if(width == width_ && height == height_) return true;

      width_ = width;
      height_ = height;
      resolution_ = width * height * channels_;
      texture_->bind();
      texture_->allocate(width_, height_, nullptr, data_format_,
                         GL_UNSIGNED_BYTE, internal_format_);

      return true;
    }

    void image_stream::unmap(){
      if(!mapped_) return;

      const std::size_t write_pbo = (read_pbo_ + 1) % 2;
      if(pointer_) pbo_[write_pbo].unmap();
      pbo_[write_pbo].release();
      mapped_ = false;

      window_->force_change();
    }

    bool image_stream::upload_data(const void *data, std::size_t buffer_size){
      if(mapped_) unmap();

      // Frames get written after dequeuing the buffer
      read_pbo_ = (read_pbo_ + 1) % 2;
      const std::size_t write_pbo = (read_pbo_ + 1) % 2;

      bool ok{true};

      if(buffer_size > static_cast<std::size_t>(resolution_)){
        buffer_size = resolution_;
        ok = false;
      }

      texture_->bind();
      pbo_[read_pbo_].bind();
      texture_->allocate_sub_2d(width_, height_);

      pbo_[write_pbo].bind();
      pbo_[write_pbo].allocate_data(nullptr, resolution_, GL_STREAM_DRAW);
      char* ptr = (char*)pbo_[write_pbo].map(GL_WRITE_ONLY);
      if(ptr != nullptr){
        std::memcpy(ptr, data, buffer_size);
        pbo_[write_pbo].unmap();
      }else
        ok = false;

      pbo_[write_pbo].release();

      if(ok) window_->force_change();

      return ok;
    }

    // ::::::::::::::::::::::::::::::::::: PROTECTED FUNCTIONS :::::::::::::::::::::::::::::::::::

    // :::::::::::::::::::::::::::::::::::: PRIVATE FUNCTIONS ::::::::::::::::::::::::::::::::::::

  } // namespace: gui
} // namespace: ramrod

