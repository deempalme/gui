#ifndef RAMROD_GUI_ELEMENT_H
#define RAMROD_GUI_ELEMENT_H

#include <cstdint>

#include "ramrod/gl/buffer.h"
#include "ramrod/gui/type_events.h"
#include "ramrod/gui/types.h"

namespace ramrod {
  namespace gui {
    class window;

    class element
    {
    public:
      element(gui::window *window, const float width, const float height,
              const float position_x, const float position_y,
              const int z_index = 0, const std::size_t tab_index = 0);
      /**
       * @brief Setting the color of this element
       *
       * This will automatically normalize the input arguments starting from 0 to the
       * maximum input value
       *
       * @param r Red channel value
       * @param g Green channel value
       * @param b Blue channel value
       * @param a Alpha channel value
       */
      void color(const float r, const float g, const float b, const float a);
      /**
       * @brief Hides this element
       */
      void hide();
      /**
       * @brief Getting this element's id
       *
       * @return Numeric identifier for this element
       */
      std::size_t id();
      virtual void paint();
      /**
       * @brief Getting the current position of this text input
       *
       * @return The current text input's position
       */
      gui::position<float> position();
      /**
       * @brief Setting a new position for this text input
       *
       * @param x Horizontal position from the top-left corner [in pixels]
       * @param y Vertical position from the top-left corner [in pixels]
       */
      void position(const float x, const float y);
      /**
       * @brief Setting the element offset
       *
       * You could choose between pixels with values above 1.0f or percentage with
       * values from 0.0f to 1.0f (relative to the element size)
       *
       * @param x Horizontal offset from the top-left corner of this element
       * @param y Vertical offset from the top-left corner of this element
       */
      void offset(const float x, const float y);
      /**
       * @brief Shows this element
       */
      void show();
      /**
       * @brief Getting the current size of this text input
       *
       * @return The current text input's size
       */
      gui::size<float> size();
      /**
       * @brief Setting a new size for this text input
       *
       * @param width  Text input's width [in pixels]
       * @param height Text input's height [in pixels]
       * @return `false` if width or height are equal or less than zero
       */
      bool size(const float width, const float height);

      virtual GLuint texture_id();
      virtual void texture_id(const GLuint new_texture_id);

      void texture_coordinates(const float bottom_left_x, const float bottom_left_y,
                               const float top_right_x, const float top_right_y);

      void z_index(const int new_z_index);
      int z_index();

    protected:

      virtual void update_buffer();

      gui::window *window_;

      bool display_;
      std::size_t id_;
      gui::position<float> position_;
      gui::size<float> size_;
      std::size_t tab_index_;
      bool update_;
      int z_index_;

      GLuint texture_id_;
      GLuint shader_id_;
      gl::buffer buffer_;
      gui::texture_uv uv_;
      gui::color color_;
      gui::position<float> offset_;
    };
  } // namespace: gui
} // namespace: ramrod

#endif // RAMROD_GUI_ELEMENT_H
