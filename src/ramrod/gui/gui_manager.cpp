#include "ramrod/gui/gui_manager.h"

#include <cstring>
#include <cmath>

#include "ramrod/console.h"
#include "ramrod/gl/error.h"
#include "ramrod/gl/frame_buffer.h"
#include "ramrod/gl/pixel_buffer.h"
#include "ramrod/gl/shader.h"
#include "ramrod/gl/texture.h"
#include "ramrod/gl/uniform_buffer.h"
#include "ramrod/gui/constants.h"
#include "ramrod/gui/element.h"
#include "ramrod/gui/enumerators.h"
#include "ramrod/gui/image_loader.h"
#include "ramrod/gui/window.h"

namespace ramrod {
  namespace gui {
    gui_manager::gui_manager(gui::window *parent) :
      parent_(parent),
      window_binding_{nullptr},
      global_font_size_{16},
      text_shader_{nullptr},
      t_u_color_{0},
      t_u_parent_id_{0},
      t_u_size_{0},
      active_input_{nullptr},
      current_tab_{tab_list_.begin()},
      sprite_(nullptr),
      s_u_background_color_{0},
      s_u_border_color_{0},
      s_u_id_{0},
      window_ids_{nullptr},
      window_elements_{nullptr},
      window_buffer_{nullptr},
      pixel_data_(),
      identifier_()
    {}

    gui_manager::~gui_manager(){
      if(sprite_) delete sprite_;
      if(window_binding_) delete window_binding_;
      if(sprite_shader_) delete sprite_shader_;
      if(text_shader_) delete text_shader_;
      if(window_buffer_) delete window_buffer_;
      if(window_elements_) delete window_elements_;
      if(window_ids_) delete window_ids_;
    }

    bool gui_manager::activate_input(const std::size_t id){
      if(id == 0){
        active_input_ = nullptr;
        current_tab_ = tab_list_.end();
        return true;
      }
      std::map<const std::size_t, gui::input*>::iterator it{tab_list_.find(id)};
      if(it == tab_list_.end()) return false;
      current_tab_ = it;
      active_input_ = it->second;
      return true;
    }

    bool gui_manager::activate_input(gui::input *input){
      if(!input){
        active_input_ = nullptr;
        current_tab_ = tab_list_.end();
        return false;
      }
      for(auto it = tab_list_.begin(); it != tab_list_.end(); ++it)
        if(it->second == input){
          current_tab_ = it;
          active_input_ = it->second;
          return true;
        }
      return false;
    }

    gui::input *gui_manager::active_input(){
      return active_input_;
    }

    bool gui_manager::add_font(const std::string &font_distance_path,
                               const std::string &font_info_path){
      gui::font_family new_font(font_distance_path, font_info_path);
      if(new_font.error()) return false;
      fonts_.emplace(new_font.font_name(), new_font);
      return true;
    }

    bool gui_manager::delete_font(const std::string &font_name){
      std::map<const std::string, gui::font_family>::iterator found{fonts_.find(font_name)};
      if(found == fonts_.end()) return false;
      fonts_.erase(found);
      return true;
    }

    const gui::identifier *gui_manager::read_pixel(const int x, const int y){
      window_elements_->read_buffer();
      // The buffer information starts from the bottom-left corner
      const int inverted_y{parent_->height() - y};
      window_buffer_->bind();
      window_elements_->read_pixel(static_cast<GLvoid*>(pixel_data_.data), x, inverted_y,
                                   1, 1, GL_FLOAT, GL_BGRA);
      float *ptr = static_cast<float*>(window_buffer_->map());
      if(ptr != nullptr){
        std::memcpy(pixel_data_.data, ptr, gui::byte_sizes::float_4D);
        window_buffer_->unmap();
      }
      window_buffer_->release();

      identifier_.element = (std::size_t)std::round(pixel_data_.color.b
                                                    * gui::identifiers::max_identifier_count)
                            * (std::size_t)std::round(pixel_data_.color.g
                                                      * gui::identifiers::max_identifier_count);
      identifier_.children = (std::size_t)std::round(pixel_data_.color.r
                                                     * gui::identifiers::max_identifier_count);
      identifier_.pointing_next = pixel_data_.color.a > 0.5f;

//      rr::formatted("mouse: (%.5f, %.5f, %.5f, %.5f)\n", rr::message::warning,
//                    pixel_data_.color.r, pixel_data_.color.g,
//                    pixel_data_.color.b, pixel_data_.color.a);

      return &identifier_;
    }

    gui::input *gui_manager::tab_next(){
      if(last_tab_id_ == 0) return nullptr;

      if(++current_tab_ == tab_list_.end())
        return active_input_ = (current_tab_ = tab_list_.begin())->second;
      return active_input_ = current_tab_->second;
    }

    gui::input *gui_manager::tab_previous(){
      if(last_tab_id_ == 0) return nullptr;

      if(current_tab_ == tab_list_.begin())
        return active_input_ = (current_tab_ = --tab_list_.end())->second;
      return active_input_ = (--current_tab_)->second;
    }

    // ::::::::::::::::::::::::::::::::::::: STATIC FUNCTIONS :::::::::::::::::::::::::::::::::::::

    std::size_t gui_manager::create_new_tab_input(gui::input *input, const std::size_t index){
      if(!input) return 0;
      if(index == 0){
        tab_list_.emplace(++last_tab_id_, input);
        return last_tab_id_;
      }
      tab_list_.emplace(index, input);
      return index;
    }

    gui::font_family *gui_manager::get_font(const std::string &font_name){
      if(default_font_ == nullptr)
        if(fonts_.size() > 0) default_font_ = &fonts_.begin()->second;

      if(font_name.size() == 0) return default_font_;

      std::map<const std::string, gui::font_family>::iterator found{fonts_.find(font_name)};
      if(found == fonts_.end())
         return nullptr;
      return &found->second;
    }

    std::size_t gui_manager::insert_new_element(gui::element *new_element){
      if(!new_element) return 0;
      elements_.emplace(++last_element_id_, new_element);
      return last_element_id_;
    }

    std::size_t gui_manager::last_element_id(){
      return last_element_id_;
    }

    std::size_t gui_manager::last_tab_id(){
      return last_tab_id_;
    }

    bool gui_manager::move_tab_input(gui::input *input, const std::size_t new_index){
      bool found{false};
      for(auto it = tab_list_.begin(); it != tab_list_.end(); ++it)
        if(it->second == input){
          tab_list_.erase(it);
          found = true;
        }
      tab_list_.emplace(new_index, input);
      return found;
    }

    bool gui_manager::remove_element(gui::element *old_element){
      if(!old_element) return false;
      for(auto it = elements_.begin(); it != elements_.end(); ++it)
        if(it->second == old_element){
          if(it->first == last_element_id_) --last_element_id_;
          elements_.erase(it);
          return true;
        }
      return false;
    }

    bool gui_manager::remove_tab_index(gui::input *old_input){
      if(!old_input) return false;
      for(auto it = tab_list_.begin(); it != tab_list_.end(); ++it)
        if(it->second == old_input){
          if(it->first == last_tab_id_)
            if(tab_list_.count(it->first) == 0) --last_tab_id_;
          tab_list_.erase(it);
          return true;
        }
      return false;
    }

    const gl::shader *gui_manager::sprite_shader(){
      return sprite_shader_;
    }

    // :::::::::::::::::::::::::::::::::::: PROTECTED FUNCTIONS :::::::::::::::::::::::::::::::::::

    void gui_manager::initialize(){}

    void gui_manager::load_default_font(){
      // Loading the default font
      gui::font_family rubik_medium(gui::rubik_medium_image, gui::rubik_medium_info);
      fonts_.emplace(rubik_medium.font_name(), rubik_medium);
    }

    void gui_manager::main_initialize(){
      GLenum error;
      load_default_font();
      get_font("");

      if((error = gl::error::code()) != GL_NO_ERROR)
        rr::formatted("Error before: %d -> %s Extra: %s\n", error,
                      gl::error::message(error).c_str(), gl::error::message().c_str());

      if(!sprite_){
        gui::image_loader new_image(gui::default_sprite_path, false);

        if(new_image.data()){
          sprite_ = new ramrod::gl::texture(true, gui::texture::sprite, false);
          sprite_->bind();
          sprite_->parameter(GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST);
          sprite_->allocate(new_image.width(), new_image.height(), new_image.data(),
                            new_image.format(), GL_UNSIGNED_BYTE, new_image.internal_format());
          sprite_->release();
        }
      }/*
      if(!text_shader_){
        text_shader_ = new ramrod::gl::shader(gui::text_shader_vert,
                                              gui::text_shader_frag,
                                              gui::text_shader_geom);

        if(text_shader_->error()){
          rr::error() << "gui_manager: " << text_shader_->error_log() << rr::endl;
        }else{
          text_shader_->use();
          text_shader_->set_value(text_shader_->uniform_location("u_atlas"),
                                  gui::texture::font_atlas);
          t_u_color_ = text_shader_->uniform_location("u_color");
          t_u_size_ = text_shader_->uniform_location("u_size");
        }
      }*/
      if(!sprite_shader_){
        sprite_shader_ = new ramrod::gl::shader(gui::sprite_shader_vert, gui::sprite_shader_frag);

        if(sprite_shader_->error()){
          rr::error() << "gui_manager: sprite " << sprite_shader_->error_log() << rr::endl;
        }else{
          sprite_shader_->use();
          sprite_shader_->set_value(sprite_shader_->uniform_location("u_image"),
                                    gui::texture::sprite);
        }
      }
      if(!window_elements_){
        window_elements_ = new gl::frame_buffer(true, false);
        window_elements_->bind();

        window_ids_ = new gl::texture(true, gui::texture::ids_frame, false);
        window_ids_->bind();
        window_ids_->parameter(GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST);
        window_ids_->allocate(parent_->width(), parent_->height(), nullptr, GL_BGRA,
                              GL_FLOAT, GL_RGBA32F);
        window_ids_->release();

        window_elements_->attach_2D(window_ids_->id(),
                                    GL_COLOR_ATTACHMENT0 + gui::framebuffer::ids);
        // Defining which framebuffers will be shown
//        GLenum buffers[2] = { gui::framebuffer::display, gui::framebuffer::ids };
//        window_elements_->draw_buffers(2, buffers);
        window_elements_->draw_buffer();

        if(window_elements_->status() != GL_FRAMEBUFFER_COMPLETE)
          rr::error() << "window_elements_: " << window_elements_->status_msg() << rr::endl;

        window_elements_->release();

        // Pixel buffer for fast pixel readings
        window_buffer_ = new gl::pixel_buffer(true);
        window_buffer_->bind();
        window_buffer_->allocate_data(nullptr, gui::byte_sizes::float_4D);
        window_buffer_->release();
      }
      if(!window_binding_){
        const float u_window[3] = { static_cast<float>(parent_->width()),
                                    static_cast<float>(parent_->height()),
                                    static_cast<float>(global_font_size_) };

        window_binding_ = new gl::uniform_buffer(true);
        window_binding_->bind();
        window_binding_->allocate(u_window, gui::uniform_buffer::window_binding_size,
                                  GL_STATIC_DRAW);
        window_binding_->release();
        window_binding_->bind_base(gui::uniform_buffer::window);

        const GLchar *names[] = {"u_window_size", "u_font_size"};
        GLuint values[2];
        glGetUniformIndices(sprite_shader_->id(), 2, names, values);
        rr::formatted("uws: %u, ufs: %u\n", rr::message::warning, values[0], values[1]);

        GLint offsets[2];
        glGetActiveUniformsiv(sprite_shader_->id(), 2, values, GL_UNIFORM_OFFSET, offsets);
        rr::formatted("uwso: %d, ufso: %d\n", rr::message::warning, offsets[0], offsets[1]);

        rr::formatted("uws: %d\n", rr::message::warning,
                      sprite_shader_->uniform_block_data_size("u_window"));
        rr::formatted("uwb: %d\n", rr::message::warning,
                      sprite_shader_->uniform_block_binding("u_window"));
        rr::formatted("uwi: %d\n", rr::message::warning,
                      sprite_shader_->uniform_block_index("u_window"));
      }
      if((error = gl::error::code()) != GL_NO_ERROR)
        rr::formatted("Error after: %d -> %s\n", error, gl::error::message(error).c_str());
    }

    void gui_manager::paint(){
      sprite_shader_->use();
      sprite_->bind();
      sprite_->activate();
      for(const std::pair<const std::size_t, gui::element*> &element : elements_)
        if(!element.second->hidden()){
          element.second->paint();
        }
    }

    void gui_manager::resize(){
      window_ids_->bind();
      window_ids_->allocate(parent_->width(), parent_->height(), nullptr, GL_RGBA,
                            GL_FLOAT, GL_RGBA32F);
      window_ids_->release();

      const float u_window[3] = { static_cast<float>(parent_->width()),
                                  static_cast<float>(parent_->height()),
                                  static_cast<float>(global_font_size_) };

      window_binding_->bind();
      window_binding_->allocate_section(u_window, gui::uniform_buffer::window_size_bytes,
                                        gui::uniform_buffer::window_size_offset);
      window_binding_->release();
    }
  } // namespace: gui
} // namespace: ramrod
