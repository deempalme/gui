#include "ramrod/gui/input_text.h"

#include "ramrod/gl/shader.h"
#include "ramrod/gui/constants.h"    // for float_4D, float_2D
#include "ramrod/gui/enumerators.h"  // for position, texture

namespace ramrod {
  namespace gui {
    input_text::input_text(const std::string &initial_text,
                           const float width, const float height,
                           const float position_x, const float position_y,
                           const std::size_t tab_index) :
      gui::input(initial_text, width, height, position_x, position_y, tab_index),
      background_buffer_(true)
    {
      background_buffer_.vertex_bind();
      background_buffer_.generate_array();
      background_buffer_.generate_element();
      background_buffer_.buffer_bind();

      background_buffer_.allocate_array(nullptr, sizeof(gui::point_4d<float>) * 16);

      background_buffer_.attributte_buffer(attribute::position, vector_size::vector_2D,
                                           0, byte_sizes::float_4D);
      background_buffer_.enable(attribute::position);
      background_buffer_.attributte_buffer(attribute::texture, vector_size::vector_2D,
                                           byte_sizes::float_2D, byte_sizes::float_4D);
      background_buffer_.enable(attribute::texture);

      const std::uint8_t data[] = {
        // first row
        7, 3, 6, 2, 5, 1, 4, 0, 0,
        // second row
        11, 11, 7, 10, 6, 9, 5, 8, 4, 4,
        // third row
        15, 15, 11, 14, 10, 13, 9, 12, 8
      };

      background_buffer_.allocate_element(data, sizeof(data));
      background_buffer_.vertex_release();

      change_size();
    }

    input_text::~input_text(){}

    void input_text::paint(){
      // Painting the background
      sprite_shader_->set_value(u_background_color_, background_color());
      sprite_shader_->set_value(u_border_color_, border_color());
      sprite_shader_->set_value(u_id_, static_cast<int>(id()));
      background_buffer_.vertex_bind();
      background_buffer_.draw_elements(GL_TRIANGLE_STRIP);
      background_buffer_.vertex_release();
      // Painting the text
      gui::input::paint();
    }

    // ::::::::::::::::::::::::::::::::::::: PRIVATE FUNCTIONS ::::::::::::::::::::::::::::::::::::

    void input_text::change_size(){
      gui::input::change_size();

      const float width = 1.0f;
      const float height = 1.0f;

      const float u1 = static_cast<float>(gui::sprite::input_text_u_1)
                       / static_cast<float>(gui::sprite::width);
      const float v1 = static_cast<float>(gui::sprite::input_text_v_1)
                       / static_cast<float>(gui::sprite::width);
      const float u2 = static_cast<float>(gui::sprite::input_text_u_2)
                       / static_cast<float>(gui::sprite::width);
      const float v2 = static_cast<float>(gui::sprite::input_text_v_2)
                       / static_cast<float>(gui::sprite::width);
      const float border_u = static_cast<float>(gui::sprite::input_text_border)
                             / static_cast<float>(gui::sprite::width);
      const float border_v = static_cast<float>(gui::sprite::input_text_border)
                             / static_cast<float>(gui::sprite::height);
      const float border_x = gui::sprite::input_text_border / width;
      const float border_y = gui::sprite::input_text_border / height;

      const float top = position_.y / height;
      const float left = position_.x / width;
      const float right = left + size_.width / width;
      const float bottom = top + size_.height / height;

      const gui::point_4d<float> points[16] = {
        // top
        {left,             top,               u1,            v1           }, // left
        {left  + border_x, top,               u1 + border_u, v1           }, // middle-left
        {right - border_x, top,               u2 - border_u, v1           }, // middle-right
        {right,            top,               u2,            v1           }, // right
        // middle-top
        {left,             top + border_y,    u1,            v1 + border_v}, // left
        {left  + border_x, top + border_y,    u1 + border_u, v1 + border_v}, // middle-left
        {right - border_x, top + border_y,    u2 - border_u, v1 + border_v}, // middle-right
        {right,            top + border_y,    u2,            v1 + border_v}, // right
        // middle-bottom
        {left,             bottom - border_y, u1,            v2 - border_v}, // left
        {left  + border_x, bottom - border_y, u1 + border_u, v2 - border_v}, // middle-left
        {right - border_x, bottom - border_y, u2 - border_u, v2 - border_v}, // middle-right
        {right,            bottom - border_y, u2,            v2 - border_v}, // right
        // bottom
        {left,             bottom,            u1,            v2           }, // left
        {left  + border_x, bottom,            u1 + border_u, v2           }, // middle-left
        {right - border_x, bottom,            u2 - border_u, v2           }, // middle-right
        {right,            bottom,            u2,            v2           }  // right
      };

      background_buffer_.vertex_bind();
      background_buffer_.buffer_bind();
      background_buffer_.allocate_section_array(points, sizeof(points));
      background_buffer_.vertex_release();
    }
  } // namespace: gui
} // namespace: ramrod
