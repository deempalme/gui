#include "ramrod/gui/gui_manager.h"

#include "ramrod/console.h"
#include "ramrod/gl/buffer.h"
#include "ramrod/gl/error.h"
#include "ramrod/gl/frame_buffer.h"
#include "ramrod/gl/shader.h"
#include "ramrod/gl/texture.h"
#include "ramrod/gl/uniform_buffer.h"
#include "ramrod/gui/constants.h"
#include "ramrod/gui/element.h"
#include "ramrod/gui/enumerators.h"
#include "ramrod/gui/font_loader.h"
#include "ramrod/gui/image_loader.h"
#include "ramrod/gui/input.h"
#include "ramrod/gui/window.h"

namespace ramrod {
  namespace gui {
    gui_manager::gui_manager(gui::window *window) :
      window_(window),
      fonts_(),
      global_font_size_{16},
      text_shader_{nullptr},
      t_u_color_{0},
      t_u_size_{0},
      sprite_shader_{nullptr},
      s_u_parent_id_{0},
      s_u_whole_{0},
      frame_shader_{nullptr},
      frame_buffer_{nullptr},
      back_texture_{nullptr},
      front_texture_{nullptr},
      scene_uniform_{nullptr},
      unitary_buffer_{nullptr},
      sprite_{nullptr},
      sprite_height_{0},
      sprite_width_{0},
      using_elements_{false},
      elements_(),
      tab_list_(),
      z_index_list_(),
      last_element_id_{0},
      last_tab_id_{0},
      last_z_index_{0},
      last_shader_id_{0},
      last_texture_id_{0},
      last_bound_texture_id_{0},
      ids_(),
      width_{0},
      height_{0},
      width_factor_{0.0f},
      height_factor_{0.0f}
    {
    }

    gui_manager::~gui_manager(){
      if(sprite_) delete sprite_;
      if(text_shader_) delete text_shader_;
      if(sprite_shader_) delete sprite_shader_;
      if(frame_shader_) delete frame_shader_;
      if(frame_buffer_) delete frame_buffer_;
      if(back_texture_) delete back_texture_;
      if(front_texture_) delete front_texture_;
      if(scene_uniform_) delete scene_uniform_;
      if(unitary_buffer_) delete unitary_buffer_;
    }

    std::size_t gui_manager::add_element(gui::element *new_element){
      if(new_element->id() != 0) return 0;

      auto success = elements_.emplace(last_element_id_ + 1, new_element);
      if(success.second){
        using_elements_ = last_element_id_ >= 0;
        return ++last_element_id_;
      }
      else
        return 0;
    }

    std::size_t gui_manager::add_tab_index(gui::input *input, const std::size_t new_tab_index){
      for(auto it = tab_list_.begin(); it != tab_list_.end(); ++it)
        if(it->second->id() == input->id()){
          tab_list_.erase(it);
          break;
        }

      if(new_tab_index == 0){
        tab_list_.emplace(++last_tab_id_, input);
        return last_tab_id_;
      }else{
        tab_list_.emplace(new_tab_index, input);
        return new_tab_index;
      }
    }

    int gui_manager::add_z_index(gui::element *element, const int new_z_index){
      if(element->id() != 0)
        for(auto it = z_index_list_.begin(); it != z_index_list_.end(); ++it)
          if(it->second->id() == element->id()){
            z_index_list_.erase(it);
            break;
          }

      if(new_z_index == 0){
        z_index_list_.emplace(++last_z_index_, element);
        return last_z_index_;
      }else{
        z_index_list_.emplace(new_z_index, element);
        return new_z_index;
      }
    }

    const pixel_id &gui_manager::calculate_ids(int x, int y){
      int data[4];
      x *= width_factor_;
      y *= height_factor_;

      frame_buffer_->bind();
      glReadBuffer(GL_COLOR_ATTACHMENT0 + gui::framebuffer::back);
      glReadPixels(x, gui::resolution::full_hd_height - 1 - y, 1, 1, GL_RGBA_INTEGER,  GL_INT, data);
      frame_buffer_->release();

      if(data[1] == 0){
        ids_.parent = 0;
        ids_.object_id = 0;
        ids_.object = nullptr;
        ids_.object_x = 0;
        ids_.object_y = 0;
        return ids_;
      }else if(static_cast<std::size_t>(data[1]) == ids_.object_id)
        return ids_;

      ids_.parent = static_cast<std::size_t>(data[0]);
      ids_.object_id = static_cast<std::size_t>(data[1]);
//      ids_.object = elements_[ids_.object_id];
      ids_.object_x = static_cast<std::uint32_t>(data[2]);
      ids_.object_y = static_cast<std::uint32_t>(data[3]);

      return ids_;
    }

    void gui_manager::bind_shader(const GLuint shader_id){
      if(last_shader_id_ == shader_id) return;
      last_shader_id_ = shader_id;
      glUseProgram(shader_id);
    }

    void gui_manager::bind_texture(const GLuint texture_id, const GLuint texture_active){
      if(last_bound_texture_id_ == texture_id) return;
      last_bound_texture_id_ = texture_id;
      glActiveTexture(GL_TEXTURE0 + texture_active);
      glBindTexture(GL_TEXTURE_2D, last_bound_texture_id_);
    }

    std::size_t gui_manager::hovered_element(){
      return ids_.object_id;
    }

    std::size_t gui_manager::last_element_id(){
      return last_element_id_;
    }

    std::size_t gui_manager::last_tab_id(){
      return last_tab_id_;
    }

    GLuint gui_manager::last_texture_id(){
      return last_texture_id_;
    }

    int gui_manager::last_z_index(){
      return last_z_index_;
    }

    std::size_t gui_manager::modify_tab_index(gui::input *input, const std::size_t new_tap_index){
      if(input->tab_index() == new_tap_index) return new_tap_index;

      for(auto it = tab_list_.begin(); it != tab_list_.end(); ++it){
        if(it->second->id() == input->id()){
          tab_list_.erase(it);
          tab_list_.emplace(new_tap_index, input);
          return new_tap_index;
        }
      }
      return 0;
    }

    int gui_manager::modify_z_index(gui::element *element, const int new_z_index){
      if(element->z_index() == new_z_index) return new_z_index;

      for(auto it = z_index_list_.begin(); it != z_index_list_.end(); ++it){
        if(it->second->id() == element->id()){
          z_index_list_.erase(it);
          z_index_list_.emplace(new_z_index, element);
          return new_z_index;
        }
      }
      return 0;
    }

    bool gui_manager::remove_element(gui::element *old_element){
      for(auto iterator = elements_.begin(); iterator != elements_.end(); ++iterator){
        if(iterator->second->id() == old_element->id()){
          elements_.erase(iterator);
          using_elements_ = elements_.size() > 0;
          return true;
        }
      }
      return false;
    }

    bool gui_manager::remove_tab_index(gui::input *old_input){
      for(auto iterator = tab_list_.begin(); iterator != tab_list_.end(); ++iterator){
        if(iterator->second->id() == old_input->id()){
          tab_list_.erase(iterator);
          return true;
        }
      }
      return false;
    }

    bool gui_manager::remove_z_index(gui::element *old_element){
      for(auto iterator = z_index_list_.begin(); iterator != z_index_list_.end(); ++iterator)
        if(iterator->second->id() == old_element->id()){
          z_index_list_.erase(iterator);
          return true;
        }
      return false;
    }

    bool gui_manager::new_sprite(const std::string &sprite_path){
      gui::image_loader new_image(sprite_path, false);

      if(new_image.data()){
        sprite_->bind();
        sprite_->allocate(new_image.width(), new_image.height(), new_image.data(),
                          new_image.format(), GL_UNSIGNED_BYTE, new_image.internal_format());
        sprite_->release();
        sprite_->activate();
        sprite_height_ = static_cast<float>(new_image.height());
        sprite_width_ = static_cast<float>(new_image.width());
        return true;
      }
      return false;
    }

    GLuint gui_manager::sprite_id(){
      return sprite_->id();
    }

    float gui_manager::sprite_height(){
      return sprite_height_;
    }

    float gui_manager::sprite_width(){
      return sprite_width_;
    }

    GLuint gui_manager::sprite_shader(){
      return sprite_shader_->id();
    }

    // :::::::::::::::::::::::::::::::::::: PROTECTED FUNCTIONS :::::::::::::::::::::::::::::::::::

    void gui_manager::initialize(){
      // Loading the default fonts
      gui::font_loader rubik_regular(gui::rubik_regular_image, gui::rubik_regular_info);
      gui::font_loader rubik_medium(gui::rubik_medium_image, gui::rubik_medium_info);

      fonts_.emplace(rubik_regular.font_name(), rubik_regular);
      fonts_.emplace(rubik_medium.font_name(), rubik_medium);

      if(!sprite_){
        gui::image_loader new_image(gui::default_sprite_path, false);

        if(new_image.data()){
          sprite_ = new ramrod::gl::texture(true, gui::texture_unit::sprite);
          sprite_->bind();
          sprite_->parameter(GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST);
          sprite_->allocate(new_image.width(), new_image.height(), new_image.data(),
                            new_image.format(), GL_UNSIGNED_BYTE, new_image.internal_format());
          sprite_->release();
          sprite_->activate();
          sprite_height_ = static_cast<float>(new_image.height());
          sprite_width_ = static_cast<float>(new_image.width());
        }
      }
      if(!text_shader_){
//        text_shader_ = new ramrod::gl::shader(gui::text_shader_vert,
//                                              gui::text_shader_frag,
//                                              gui::text_shader_geom);

//        if(text_shader_->error()){
//          rr::error() << "gui_manager text_shader: " << text_shader_->error_log() << rr::endl;
//        }else{
//          text_shader_->use();
//          text_shader_->set_value(text_shader_->uniform_location("u_atlas"),
//                                  gui::texture_unit::font_atlas);
//          t_u_color_ = text_shader_->uniform_location("u_color");
//          t_u_size_ = text_shader_->uniform_location("u_size");
//        }
      }
      if(!sprite_shader_){
        sprite_shader_ = new ramrod::gl::shader(gui::shader::sprite_shader_vert,
                                                gui::shader::sprite_shader_frag,
                                                gui::shader::sprite_shader_geom);
        if(sprite_shader_->error()){
          rr::error() << "gui_manager image_shader: " << sprite_shader_->error_log() << rr::endl;
        }else{
          sprite_shader_->use();
          sprite_shader_->set_value(sprite_shader_->uniform_location("u_image"),
                                    gui::texture_unit::sprite);
          s_u_parent_id_ = sprite_shader_->uniform_location("u_parent_id");
          sprite_shader_->set_value(s_u_parent_id_, 0);
          s_u_whole_ = sprite_shader_->uniform_location("u_whole");
        }
      }
      if(!back_texture_){
        back_texture_ = new gl::texture(true, gui::texture_unit::background_ids, false);
        back_texture_->bind();
        back_texture_->parameter(GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST);
        back_texture_->allocate(gui::resolution::full_hd_width, gui::resolution::full_hd_height,
                                nullptr, GL_RGBA_INTEGER, GL_INT, GL_RGBA32I);
        back_texture_->release();
      }
      if(!front_texture_){
        front_texture_ = new gl::texture(true, gui::texture_unit::front_frame, false);
        front_texture_->bind();
        front_texture_->parameter(GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST);
        front_texture_->allocate(gui::resolution::full_hd_width, gui::resolution::full_hd_height,
                                 nullptr);
        front_texture_->release();
      }
      if(!frame_buffer_){
        frame_buffer_ = new gl::frame_buffer(true);
        frame_buffer_->bind();
        frame_buffer_->attach_2D(front_texture_->id(), GL_COLOR_ATTACHMENT0 + gui::framebuffer::front);
        frame_buffer_->attach_2D(back_texture_->id(), GL_COLOR_ATTACHMENT0 + gui::framebuffer::back);

        GLenum buffers[] = { GL_COLOR_ATTACHMENT0 + gui::framebuffer::front,
                             GL_COLOR_ATTACHMENT0 + gui::framebuffer::back };
        frame_buffer_->draw_buffers(2, buffers);

        if(frame_buffer_->status() != GL_FRAMEBUFFER_COMPLETE)
          rr::error() << "frame_buffer_: " << frame_buffer_->status_msg() << rr::endl;

        frame_buffer_->release();
      }
      if(!scene_uniform_){
        scene_uniform_ = new gl::uniform_buffer(true);
        scene_uniform_->bind();
        scene_uniform_->allocate(nullptr, gui::byte_sizes::float_4D);
        scene_uniform_->bind_base(gui::uniform_buffer::scene);
        scene_uniform_->release();
      }
      if(!unitary_buffer_){
        const float data[] = {
          // Position  // Texture coordinates
          1.0f, 1.0f,  1.0f, 0.0f,
          1.0f, 0.0f,  1.0f, 1.0f,
          0.0f, 1.0f,  0.0f, 0.0f,
          1.0f, 0.0f,  1.0f, 1.0f,
          0.0f, 0.0f,  0.0f, 1.0f,
          0.0f, 1.0f,  0.0f, 0.0f
        };

        unitary_buffer_ = new gl::buffer(true);
        unitary_buffer_->vertex_bind();
        unitary_buffer_->generate_array();
        unitary_buffer_->buffer_bind();

        unitary_buffer_->allocate_array(data, sizeof(data));

        unitary_buffer_->attributte_buffer(gui::attribute::position, gui::vector_size::vector_2D,
                                           0, gui::byte_sizes::float_4D);
        unitary_buffer_->enable(gui::attribute::position);

        unitary_buffer_->attributte_buffer(gui::attribute::texture, gui::vector_size::vector_2D,
                                           gui::byte_sizes::float_2D, gui::byte_sizes::float_4D);
        unitary_buffer_->enable(gui::attribute::texture);

        unitary_buffer_->vertex_release();
      }
      if(!frame_shader_){
        frame_shader_ = new ramrod::gl::shader(gui::shader::frame_shader_vert,
                                               gui::shader::frame_shader_frag);
        if(frame_shader_->error()){
          rr::error() << "gui_manager frame_shader: " << frame_shader_->error_log() << rr::endl;
        }else{
          frame_shader_->use();
          frame_shader_->set_value(frame_shader_->uniform_location("u_image"),
                                   gui::texture_unit::front_frame);
        }
      }
    }

    void gui_manager::key_down_event(const keyboard_event::key &event){
      if(event.scancode == SDL_SCANCODE_F11){
        window_->full_screen(!window_->full_screen());
      }
//      rr::formatted("scancode: %d, keycode: %d, mod: %d\n", rr::message::attention,
//                    event.scancode, event.sym, event.mod);
    }

    void gui_manager::key_up_event(const keyboard_event::key &/*event*/){
//      rr::formatted("scancode: %d, keycode: %d, mod: %d\n", rr::message::attention,
//                    event.scancode, event.sym, event.mod);
    }

    void gui_manager::mouse_down_event(const gui::mouse_event::button &/*event*/){}

    void gui_manager::mouse_move_event(const gui::mouse_event::move &event){
      calculate_ids(event.x, event.y);
    }

    void gui_manager::mouse_up_event(const gui::mouse_event::button &/*event*/){}

    void gui_manager::paint(){
      pre_paint();
      for(auto &element : z_index_list_){
        element.second->paint();
      }
      post_paint();
    }

    void gui_manager::resize(const int width, const int height){
      if(width_ == width && height_ == height) return;

      width_ = width;
      height_ = height;

      const float window_size[] = { static_cast<float>(width), static_cast<float>(height) };
      const float sprite_size[] = { sprite_width_, sprite_height_ };

      width_factor_ = gui::resolution::full_hd_width / window_size[0];
      height_factor_ = gui::resolution::full_hd_height / window_size[1];

      scene_uniform_->bind();
      scene_uniform_->allocate_section(window_size, sizeof(window_size));
      scene_uniform_->allocate_section(sprite_size, sizeof(sprite_size), gui::byte_sizes::float_2D);
      scene_uniform_->release();
    }

    void gui_manager::restart_viewport(){
      glViewport(0, 0, width_, height_);
    }

    void gui_manager::pre_paint(){
      if(!using_elements_) return;

      frame_buffer_->bind();
      sprite_->bind();
      sprite_shader_->use();

      glViewport(0, 0, gui::resolution::full_hd_width, gui::resolution::full_hd_height);
      window_->screen_clear();
    }

    void gui_manager::post_paint(){
      if(!using_elements_) return;

      frame_buffer_->release();

      restart_viewport();

      frame_shader_->use();

      front_texture_->activate();
      front_texture_->bind();

      unitary_buffer_->vertex_bind();
      unitary_buffer_->draw(GL_TRIANGLES, 0, 6);
      unitary_buffer_->vertex_release();
    }
  } // namespace: gui
} // namespace: ramrod
