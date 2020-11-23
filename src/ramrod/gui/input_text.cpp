#include "ramrod/gui/input_text.h"

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
      // TODO: calculate id_
      background_buffer_.vertex_bind();
      background_buffer_.generate_array();
      background_buffer_.buffer_bind();

      background_buffer_.allocate_array(nullptr, sizeof(float) * 108);

      background_buffer_.attributte_buffer(attribute::position, vector_size::vector_2D,
                                           0, byte_sizes::float_4D);
      background_buffer_.enable(attribute::position);
      background_buffer_.attributte_buffer(attribute::texture, vector_size::vector_2D,
                                           byte_sizes::float_2D, byte_sizes::float_4D);
      background_buffer_.enable(attribute::texture);
      background_buffer_.vertex_release();

      change_size();
    }

    // ::::::::::::::::::::::::::::::::::::: PRIVATE FUNCTIONS ::::::::::::::::::::::::::::::::::::

    void input_text::change_size(){
      gui::input::change_size();

      const float u1 = static_cast<float>(gui::sprite::input_text_u_1)
                       / static_cast<float>(gui::sprite::width);
      const float v1 = static_cast<float>(gui::sprite::input_text_v_1)
                       / static_cast<float>(gui::sprite::width);
      const float u2 = static_cast<float>(gui::sprite::input_text_u_2)
                       / static_cast<float>(gui::sprite::width);
      const float v2 = static_cast<float>(gui::sprite::input_text_v_2)
                       / static_cast<float>(gui::sprite::width);
      const float border_x = static_cast<float>(gui::sprite::input_text_border)
                             / static_cast<float>(gui::sprite::width);
      const float border_y = static_cast<float>(gui::sprite::input_text_border)
                             / static_cast<float>(gui::sprite::height);
      const float border = gui::sprite::input_text_border;

      const float top = position_.x;
      const float left = position_.y;
      const float right = left + size_.width;
      const float bottom = top + size_.height;

      const gui::point_4d<float> points[16] = {
        // top
        {left,           top,             u1,            v1           }, // left
        {left  + border, top,             u1 + border_x, v1           }, // middle-left
        {right - border, top,             u2 - border_x, v1           }, // middle-right
        {right,          top,             u2,            v1           }, // right
        // middle-top
        {left,           top + border,    u1,            v1 + border_y}, // left
        {left  + border, top + border,    u1 + border_x, v1 + border_y}, // middle-left
        {right - border, top + border,    u2 - border_x, v1 + border_y}, // middle-right
        {right,          top + border,    u2,            v1 + border_y}, // right
        // middle-bottom
        {left,           bottom - border, u1,            v2 - border_y}, // left
        {left  + border, bottom - border, u1 + border_x, v2 - border_y}, // middle-left
        {right - border, bottom - border, u2 - border_x, v2 - border_y}, // middle-right
        {right,          bottom - border, u2,            v2 - border_y}, // right
        // bottom
        {left,           bottom,          u1,            v2           }, // left
        {left  + border, bottom,          u1 + border_x, v2           }, // middle-left
        {right - border, bottom,          u2 - border_x, v2           }, // middle-right
        {right,          bottom,          u2,            v2           }  // right
      };

      const gui::point_4d<float> data[27] = {
        // first row
        points[3],   points[7],  points[2],  points[6], points[1],  points[5], points[0],  points[4],  points[4],
        // second row
        points[7],  points[11],  points[6], points[10], points[5],  points[9], points[4],  points[8],  points[8],
        // third row
        points[11], points[15], points[10], points[14], points[9], points[13], points[8], points[12], points[12]
      };

      background_buffer_.vertex_bind();
      background_buffer_.buffer_bind();
      background_buffer_.allocate_section_array(data, 0, sizeof(data));
      background_buffer_.vertex_release();
    }
  } // namespace: gui
} // namespace: ramrod
