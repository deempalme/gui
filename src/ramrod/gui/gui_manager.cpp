#include "ramrod/gui/gui_manager.h"

#include "ramrod/console.h"
#include "ramrod/gl/shader.h"
#include "ramrod/gl/texture.h"
#include "ramrod/gui/constants.h"
#include "ramrod/gui/element.h"
#include "ramrod/gui/enumerators.h"
#include "ramrod/gui/font_loader.h"
#include "ramrod/gui/image_loader.h"

namespace ramrod {
  namespace gui {
    gui_manager::gui_manager(gui::window *parent) :
      parent_(parent),
      sprite_(nullptr),
      fonts_(),
      global_font_size_{16},
      elements_(),
      tab_list_(),
      text_shader_{nullptr},
      t_u_color_{0},
      t_u_size_{0},
      image_shader_{nullptr}
    {
      // Loading the default fonts
      gui::font_loader rubik_regular(gui::rubik_regular_image, gui::rubik_regular_info);
      gui::font_loader rubik_medium(gui::rubik_medium_image, gui::rubik_medium_info);

      fonts_[rubik_regular.font_name()] = rubik_regular;
      fonts_[rubik_medium.font_name()] = rubik_medium;
    }

    gui_manager::~gui_manager(){
      if(sprite_) delete sprite_;
      if(text_shader_) delete text_shader_;
      if(image_shader_) delete image_shader_;
    }

    std::size_t gui_manager::insert_new_element(element *new_element){

    }

    std::size_t gui_manager::create_new_tab_input(input *input){

    }

    bool gui_manager::remove_element(element *old_element){

    }

    bool gui_manager::remove_tab_index(input *old_input){

    }

    std::size_t gui_manager::last_element_id(){
      return last_element_id_;
    }

    std::size_t gui_manager::last_tab_id(){
      return last_tab_id_;
    }

    // :::::::::::::::::::::::::::::::::::: PROTECTED FUNCTIONS :::::::::::::::::::::::::::::::::::

    void gui_manager::initialize(){
      if(!sprite_){
        gui::image_loader new_image(gui::default_sprite_path);

        if(new_image.data()){
          sprite_ = new ramrod::gl::texture(true, gui::texture::sprite);
          sprite_->activate();
          sprite_->bind();
          sprite_->allocate(new_image.width(), new_image.height(), new_image.data(),
                            new_image.format(), GL_UNSIGNED_BYTE, new_image.internal_format());
        }
      }
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
      }
      if(!image_shader_){
        image_shader_ = new ramrod::gl::shader(gui::image_shader_vert, gui::image_shader_frag);

        if(image_shader_->error()){
          rr::error() << "gui_manager: " << image_shader_->error_log() << rr::endl;
        }else{
          image_shader_->use();
          image_shader_->set_value(image_shader_->uniform_location("u_image"),
                                   gui::texture::albedo);
        }
      }
    }

    void gui_manager::paint(bool force){}
  } // namespace: gui
} // namespace: ramrod
