#include "ramrod/gui/video_yv12.h"

#include <cstring>

#include "ramrod/console/error.h"
#include "ramrod/gl/pixel_buffer.h"
#include "ramrod/gl/shader.h"
#include "ramrod/gl/texture.h"
#include "ramrod/gui/constants.h"
#include "ramrod/gui/window.h"

namespace ramrod {
  namespace gui {
    video_yv12::video_yv12(gui::window *window,
                           const int resolution_x, const int resolution_y,
                           const float width, const float height,
                           const float position_x, const float position_y,
                           const int z_index, const std::size_t tab_index) :
      gui::element(window, width, height, position_x, position_y, z_index, tab_index),
      resolution_x_{resolution_x},
      resolution_y_{resolution_y},
      resolution_{resolution_x_ * resolution_y_},
      quarter_resolution_{resolution_ / 4},
      pbo_{static_cast<gl::pixel_buffer*>(std::malloc(sizeof(gl::pixel_buffer) * 6))},
      read_pbo_{0},
      y_tex_{new gl::texture(false, gui::texture_unit::y_tex, false)},
      u_tex_{new gl::texture(false, gui::texture_unit::u_tex, false)},
      v_tex_{new gl::texture(false, gui::texture_unit::v_tex, false)},
      inverted_{false}
    {
      if(video_yv12::elements_++ == 0){
        video_yv12::shader_ = new gl::shader();
        if(video_yv12::shader_->create(gui::shader::sprite_shader_vert,
                                       gui::shader::yv12_frag_shader,
                                       gui::shader::sprite_shader_geom)){
          video_yv12::shader_->use();
          video_yv12::shader_->set_value(shader_->uniform_location("y_tex"),
                                         gui::texture_unit::y_tex);
          video_yv12::shader_->set_value(shader_->uniform_location("u_tex"),
                                         gui::texture_unit::u_tex);
          video_yv12::shader_->set_value(shader_->uniform_location("v_tex"),
                                         gui::texture_unit::v_tex);

          shader_id_ = video_yv12::shader_->id();
        }else
          rr::error(video_yv12::shader_->error_log());
      }

      y_tex_->generate();
      y_tex_->bind();
      y_tex_->parameter(GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST);
      y_tex_->allocate(resolution_x_, resolution_y_,
                       nullptr, GL_RED, GL_UNSIGNED_BYTE, GL_R8);

      u_tex_->generate();
      u_tex_->bind();
      u_tex_->parameter(GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST);
      u_tex_->allocate(resolution_x_/2, resolution_y_/2,
                       nullptr, GL_RED, GL_UNSIGNED_BYTE, GL_R8);

      v_tex_->generate();
      v_tex_->bind();
      v_tex_->parameter(GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST);
      v_tex_->allocate(resolution_x_/2, resolution_y_/2,
                       nullptr, GL_RED, GL_UNSIGNED_BYTE, GL_R8);

      // Y's texture PBO
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

      // U's texture PBO
      pbo_[2] = gl::pixel_buffer(false, gl::pixel::buffer::unpack);
      pbo_[2].generate();
      pbo_[2].bind();
      pbo_[2].allocate_data(nullptr, quarter_resolution_, GL_STREAM_DRAW);
      pbo_[2].release();

      pbo_[3] = gl::pixel_buffer(false, gl::pixel::buffer::unpack);
      pbo_[3].generate();
      pbo_[3].bind();
      pbo_[3].allocate_data(nullptr, quarter_resolution_, GL_STREAM_DRAW);
      pbo_[3].release();

      // V's texture PBO
      pbo_[4] = gl::pixel_buffer(false, gl::pixel::buffer::unpack);
      pbo_[4].generate();
      pbo_[4].bind();
      pbo_[4].allocate_data(nullptr, quarter_resolution_, GL_STREAM_DRAW);
      pbo_[4].release();

      pbo_[5] = gl::pixel_buffer(false, gl::pixel::buffer::unpack);
      pbo_[5].generate();
      pbo_[5].bind();
      pbo_[5].allocate_data(nullptr, quarter_resolution_, GL_STREAM_DRAW);
      pbo_[5].release();
    }

    video_yv12::~video_yv12(){
      if(--video_yv12::elements_ == 0) delete video_yv12::shader_;
      if(y_tex_) delete y_tex_;
      if(u_tex_) delete u_tex_;
      if(v_tex_) delete v_tex_;
      std::free(pbo_);
    }

    void video_yv12::invert_y_axis(){
      if((inverted_ = !inverted_)){
        texture_coordinates(0, 1.0f, 1.0f, 0.0f);
      }else{
        texture_coordinates(0, 0.0f, 1.0f, 1.0f);
      }
    }

    void video_yv12::paint(){
      if(!display_) return;

      window_->bind_shader(shader_id_);

      y_tex_->activate();
      y_tex_->bind();
      u_tex_->activate();
      u_tex_->bind();
      v_tex_->activate();
      v_tex_->bind();

      buffer_.vertex_bind();
      buffer_.draw(GL_POINTS, 0, 1);
      buffer_.vertex_release();
    }

    bool video_yv12::resolution(const int width, const int height){
      if(width <= 0 || height <= 0) return false;

      if(width == resolution_x_ && height == resolution_y_) return true;

      resolution_x_ = width;
      resolution_y_ = height;
      resolution_ = resolution_x_ * resolution_y_;
      quarter_resolution_ = resolution_ / 4;

      y_tex_->bind();
      y_tex_->allocate(resolution_x_, resolution_y_,
                       nullptr, GL_RED, GL_UNSIGNED_BYTE, GL_R8);

      u_tex_->bind();
      u_tex_->allocate(resolution_x_/2, resolution_y_/2,
                       nullptr, GL_RED, GL_UNSIGNED_BYTE, GL_R8);

      v_tex_->bind();
      v_tex_->allocate(resolution_x_/2, resolution_y_/2,
                       nullptr, GL_RED, GL_UNSIGNED_BYTE, GL_R8);

      // Y's texture PBO
      pbo_[0].bind();
      pbo_[0].allocate_data(nullptr, resolution_, GL_STREAM_DRAW);
      pbo_[0].release();

      pbo_[1].bind();
      pbo_[1].allocate_data(nullptr, resolution_, GL_STREAM_DRAW);
      pbo_[1].release();

      // U's texture PBO
      pbo_[2].bind();
      pbo_[2].allocate_data(nullptr, quarter_resolution_, GL_STREAM_DRAW);
      pbo_[2].release();

      pbo_[3].bind();
      pbo_[3].allocate_data(nullptr, quarter_resolution_, GL_STREAM_DRAW);
      pbo_[3].release();

      // V's texture PBO
      pbo_[4].bind();
      pbo_[4].allocate_data(nullptr, quarter_resolution_, GL_STREAM_DRAW);
      pbo_[4].release();

      pbo_[5].bind();
      pbo_[5].allocate_data(nullptr, quarter_resolution_, GL_STREAM_DRAW);
      pbo_[5].release();

      return true;
    }

    int video_yv12::resolution_x(){
      return resolution_x_;
    }

    int video_yv12::resolution_y(){
      return resolution_y_;
    }

    GLuint video_yv12::texture_id(){
      return y_tex_->id();
    }

    bool video_yv12::upload_data(const void *data, const std::size_t offset){
      const std::size_t resolution{static_cast<std::size_t>(resolution_)};
      const std::size_t quarter_resolution{static_cast<std::size_t>(quarter_resolution_)};

      return upload_data(data, offset,
                         data, offset + resolution,
                         data, offset + resolution + quarter_resolution);
    }

    bool video_yv12::upload_data(const void *y_data, const std::size_t y_offset,
                                 const void *u_data, const std::size_t u_offset,
                                 const void *v_data, const std::size_t v_offset){
      // Frames get written after dequeuing the buffer
      read_pbo_ = (read_pbo_ + 1) % 2;
      const std::size_t write_pbo = (read_pbo_ + 1) % 2;

      bool ok{true};

      const std::size_t quarter_resolution{static_cast<std::size_t>(quarter_resolution_)};

      // Uploading Y texture data
      pbo_[read_pbo_].bind();
      y_tex_->bind();
      y_tex_->allocate_sub_2d(resolution_x_, resolution_y_,
                              nullptr, 0, 0, GL_RED, GL_UNSIGNED_BYTE);

      pbo_[write_pbo].bind();
      pbo_[write_pbo].allocate_data(nullptr, resolution_, GL_STREAM_DRAW);
      unsigned char* ptr = (unsigned char*)pbo_[write_pbo].map(GL_WRITE_ONLY);
      if(ptr){
        std::memcpy(ptr, (unsigned char*)y_data + y_offset, static_cast<std::size_t>(resolution_));
        pbo_[write_pbo].unmap();
      }else
        ok = false;
      pbo_[write_pbo].release();

      // Uploading U texture data
      pbo_[read_pbo_ + 2].bind();
      u_tex_->bind();
      u_tex_->allocate_sub_2d(resolution_x_/2, resolution_y_/2,
                              nullptr, 0, 0, GL_RED, GL_UNSIGNED_BYTE);

      pbo_[write_pbo + 2].bind();
      pbo_[write_pbo + 2].allocate_data(nullptr, quarter_resolution_, GL_STREAM_DRAW);
      ptr = (unsigned char*)pbo_[write_pbo + 2].map(GL_WRITE_ONLY);
      if(ptr){
        std::memcpy(ptr, (unsigned char*)u_data + u_offset, quarter_resolution);
        pbo_[write_pbo + 2].unmap();
      }else
        ok = false;
      pbo_[write_pbo + 2].release();

      // Uploading V texture data
      pbo_[read_pbo_ + 4].bind();
      v_tex_->bind();
      v_tex_->allocate_sub_2d(resolution_x_/2, resolution_y_/2,
                              nullptr, 0, 0, GL_RED, GL_UNSIGNED_BYTE);

      pbo_[write_pbo + 4].bind();
      pbo_[write_pbo + 4].allocate_data(nullptr, quarter_resolution_, GL_STREAM_DRAW);
      ptr = (unsigned char*)pbo_[write_pbo + 4].map(GL_WRITE_ONLY);
      if(ptr){
        std::memcpy(ptr, (unsigned char*)v_data + v_offset, quarter_resolution);
        pbo_[write_pbo + 4].unmap();
      }else
        ok = false;
      pbo_[write_pbo + 4].release();

      if(ok) window_->force_change();

      return ok;
    }
  } // namespace: gui
} // namespace: ramrod
