#include "ramrod/gui/element.h"

#include "ramrod/gui/constants.h"
#include "ramrod/gui/gui_manager.h"
#include "ramrod/gui/window.h"

namespace ramrod {
  namespace gui {
    element::element(window *window, const float width, const float height,
                     const float position_x, const float position_y,
                     const int z_index, const std::size_t tab_index) :
      window_{window},
      display_{true},
      id_{0},
      position_{position_x, position_y},
      size_{width, height},
      tab_index_{tab_index},
      update_{false},
      z_index_{0},
      texture_id_{0},
      shader_id_{0},
      buffer_(),
      uv_({ 0.0f, 0.0f, 1.0f, 1.0f }),
      color_({ 1.0f, 1.0f, 1.0f, 1.0f }),
      offset_({ 0.0f, 0.0f })
    {
      z_index_ = window_->add_z_index(this, z_index);
      id_ = window_->add_element(this);
      shader_id_ = window_->sprite_shader();

      buffer_.generate_vertex();
      buffer_.vertex_bind();
      buffer_.generate_array();
      buffer_.buffer_bind();

      buffer_.allocate_array(nullptr, gui::byte_sizes::float_15D);

      buffer_.attributte_buffer(gui::attribute::position, gui::vector_size::vector_4D,
                                0, gui::byte_sizes::float_15D);
      buffer_.enable(gui::attribute::position);
      buffer_.attributte_buffer(gui::attribute::texture, gui::vector_size::vector_4D,
                                gui::byte_sizes::float_4D, gui::byte_sizes::float_15D);
      buffer_.enable(gui::attribute::texture);
      buffer_.attributte_buffer(gui::attribute::color, gui::vector_size::vector_4D,
                                gui::byte_sizes::float_8D, gui::byte_sizes::float_15D);
      buffer_.enable(gui::attribute::color);
      buffer_.attributte_buffer(gui::attribute::offset, gui::vector_size::vector_2D,
                                gui::byte_sizes::float_12D, gui::byte_sizes::float_15D);
      buffer_.enable(gui::attribute::offset);
      buffer_.attributte_buffer(gui::attribute::id, gui::vector_size::vector_1D,
                                gui::byte_sizes::float_14D, gui::byte_sizes::float_15D);
      buffer_.enable(gui::attribute::id);
      buffer_.vertex_release();

      element::update_buffer();
    }

    void element::color(const float r, const float g, const float b, const float a){
      float max{r};
      if(g > max) max = g;
      if(b > max) max = b;
      if(a > max) max = a;
      color_.r = r / max;
      color_.g = g / max;
      color_.b = b / max;
      color_.a = a / max;
      update_ = true;
      update_buffer();
      window_->force_change();
    }

    void element::hide(){
      display_ = false;
      window_->force_change();
    }

    std::size_t element::id(){
      return id_;
    }

    void element::paint(){
      if(!display_) return;

      window_->bind_shader(shader_id_);
      // TODO: Activate too?
      window_->bind_texture(texture_id_, gui::texture_unit::sprite);

      buffer_.vertex_bind();
      buffer_.draw(GL_POINTS, 0, 1);
      buffer_.vertex_release();
    }

    gui::position<float> element::position(){
      return position_;
    }

    void element::position(const float x, const float y){
      position_.x = x;
      position_.y = y;
      update_ = true;
      update_buffer();
      window_->force_change();
    }

    void element::offset(const float x, const float y){
      offset_.x = x;
      offset_.y = y;
      update_ = true;
      update_buffer();
      window_->force_change();
    }

    void element::show(){
      display_ = true;
      window_->force_change();
    }

    gui::size<float> element::size(){
      return size_;
    }

    bool element::size(const float width, const float height){
      if((width <= 0 && height <= 0)
      || (gui::equal_floats(size_.width, width) && gui::equal_floats(size_.height, height)))
        return false;

      size_.width = width;
      size_.height = height;
      update_buffer();
      window_->force_change();
      return update_ =true;
    }

    GLuint element::texture_id(){
      return texture_id_;
    }

    void element::texture_id(const GLuint new_texture_id){
      texture_id_ = new_texture_id;
      window_->force_change();
    }

    void element::texture_coordinates(const float bottom_left_x, const float bottom_left_y,
                                      const float top_right_x, const float top_right_y){
      uv_.u1 = bottom_left_x;
      uv_.v1 = bottom_left_y;
      uv_.u2 = top_right_x;
      uv_.v2 = top_right_y;
      update_buffer();
      window_->force_change();
    }

    void element::z_index(const int new_z_index){
      z_index_ = window_->modify_z_index(this, new_z_index);
      window_->force_change();
    }

    int element::z_index(){
      return z_index_;
    }

    // :::::::::::::::::::::::::::::::::::: PROTECTED FUNCTIONS :::::::::::::::::::::::::::::::::::

    void element::update_buffer(){
      const float data[] = {
        // Position
        position_.x, position_.y,
        // Size
        size_.width, size_.height,
        // Texture coordinates
        uv_.u1, uv_.v2, uv_.u2, uv_.v1,
        // Color
        color_.r, color_.g, color_.b, color_.a,
        // Offset
        offset_.x, offset_.y,
        // Id
        static_cast<float>(id_)
      };

      buffer_.vertex_bind();
      buffer_.buffer_bind();
      buffer_.allocate_section_array(data, sizeof(data), 0);
      buffer_.vertex_release();
    }
  } // namespace: gui
} // namespace: ramrod
