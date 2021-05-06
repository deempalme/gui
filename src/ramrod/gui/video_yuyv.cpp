#include "ramrod/gui/video_yuyv.h"

#include <cstring>

#include "ramrod/console/error.h"
#include "ramrod/gl/pixel_buffer.h"
#include "ramrod/gl/shader.h"
#include "ramrod/gl/texture.h"
#include "ramrod/gui/constants.h"
#include "ramrod/gui/window.h"

namespace ramrod {
  namespace gui {
    video_yuyv::video_yuyv(gui::window *window, const float resolution_x, const float resolution_y,
                           const float width, const float height,
                           const float position_x, const float position_y,
                           const int z_index, const std::size_t tab_index) :
      gui::element(window, width, height, position_x, position_y, z_index, tab_index),
      resolution_x_{resolution_x},
      resolution_y_{resolution_y},
      i_resolution_y_{static_cast<GLsizei>(resolution_y)},
      half_resolution_x_{static_cast<GLsizei>(resolution_x * 0.5f)},
      half_resolution_{half_resolution_x_ * i_resolution_y_ * 4},
      pbo_{static_cast<gl::pixel_buffer*>(std::malloc(sizeof(gl::pixel_buffer) * 2))},
      read_pbo_{0},
      yuyv_{new gl::texture(false, gui::texture_unit::video, false)},
      inverted_{false}
    {
      if(video_yuyv::elements_++ == 0){
        video_yuyv::shader_ = new gl::shader();
        if(video_yuyv::shader_->create(gui::shader::sprite_shader_vert,
                                       gui::shader::yuyv_frag_shader,
                                       gui::shader::sprite_shader_geom)){
          video_yuyv::shader_->use();
          video_yuyv::shader_->set_value(shader_->uniform_location("yuyv_image"),
                                         gui::texture_unit::video);
          video_yuyv::u_yuyv_size_ = shader_->uniform_location("u_yuyv_size");

          shader_id_ = video_yuyv::shader_->id();
        }else
          rr::error(video_yuyv::shader_->error_log());
      }

      yuyv_->generate();
      yuyv_->bind();
      yuyv_->parameter(GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);
      yuyv_->allocate(half_resolution_x_, i_resolution_y_, nullptr, GL_RGBA,
                      GL_UNSIGNED_BYTE, GL_RGBA8);
      yuyv_->release();

      pbo_[0] = gl::pixel_buffer(false, gl::pixel::buffer::unpack);
      pbo_[0].generate();
      pbo_[0].bind();
      pbo_[0].allocate_data(nullptr, half_resolution_, GL_STREAM_DRAW);
      pbo_[0].release();

      pbo_[1] = gl::pixel_buffer(false, gl::pixel::buffer::unpack);
      pbo_[1].generate();
      pbo_[1].bind();
      pbo_[1].allocate_data(nullptr, half_resolution_, GL_STREAM_DRAW);
      pbo_[1].release();
    }

    video_yuyv::~video_yuyv(){
      if(--video_yuyv::elements_ == 0) delete video_yuyv::shader_;
      if(yuyv_) delete yuyv_;
      std::free(pbo_);
    }

    void video_yuyv::invert_y_axis(){
      if((inverted_ = !inverted_)){
        texture_coordinates(0, 1.0f, 1.0f, 0.0f);
      }else{
        texture_coordinates(0, 0.0f, 1.0f, 1.0f);
      }
    }

    void video_yuyv::paint(){
      if(!display_) return;

      window_->bind_shader(shader_id_);
      video_yuyv::shader_->set_value(video_yuyv::u_yuyv_size_, resolution_x_);

      yuyv_->activate();
      yuyv_->bind();

      buffer_.vertex_bind();
      buffer_.draw(GL_POINTS, 0, 1);
      buffer_.vertex_release();
    }

    bool video_yuyv::resolution(const float width, const float height){
      if(width <= 0 || height <= 0) return false;

      resolution_x_ = width;
      resolution_y_ = height;
      half_resolution_x_ = static_cast<GLsizei>(width * 0.5f);
      i_resolution_y_ = static_cast<GLsizei>(height);
      half_resolution_ = half_resolution_x_ * i_resolution_y_ * 4;

      yuyv_->bind();
      yuyv_->allocate(half_resolution_x_, i_resolution_y_, nullptr, GL_RGBA,
                      GL_UNSIGNED_BYTE, GL_RGBA8);
      yuyv_->release();

      pbo_[0].bind();
      pbo_[0].allocate_data(nullptr, half_resolution_, GL_STREAM_DRAW);
      pbo_[0].release();

      pbo_[1].bind();
      pbo_[1].allocate_data(nullptr, half_resolution_, GL_STREAM_DRAW);
      pbo_[1].release();

      return true;
    }

    float video_yuyv::resolution_x(){
      return resolution_x_;
    }

    float video_yuyv::resolution_y(){
      return resolution_y_;
    }

    GLuint video_yuyv::texture_id(){
      return yuyv_->id();
    }

    bool video_yuyv::upload_data(const void *data, std::size_t buffer_size,
                                 const std::size_t offset){
      // Frames get written after dequeuing the buffer
      read_pbo_ = (read_pbo_ + 1) % 2;
      const std::size_t write_pbo = (read_pbo_ + 1) % 2;

      bool ok{true};

      if((buffer_size + offset) > static_cast<std::size_t>(half_resolution_)){
        buffer_size = half_resolution_ - offset;
        ok = false;
      }

      pbo_[read_pbo_].bind();
      yuyv_->bind();
      yuyv_->allocate_sub_2d(half_resolution_x_, i_resolution_y_);

      pbo_[write_pbo].bind();
      pbo_[write_pbo].allocate_data(nullptr, half_resolution_, GL_STREAM_DRAW);
      char* ptr = (char*)pbo_[write_pbo].map(GL_WRITE_ONLY);
      if(ptr){
        std::memcpy(ptr + offset, data, buffer_size);
        pbo_[write_pbo].unmap();
      }else
        ok = false;

      pbo_[write_pbo].release();

      if(ok) window_->force_change();

      return ok;
    }
  } // namespace: gui
} // namespace: ramrod
