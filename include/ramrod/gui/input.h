#ifndef RAMROD_GUI_INPUT_H
#define RAMROD_GUI_INPUT_H

#include <cstdint>               // for size_t
#include <string>                // for string, allocator

#include "ramrod/gl/buffer.h"    // for buffer
#include "ramrod/gui/element.h"  // for position, size, selection
#include "ramrod/gui/types.h"    // for position, size, selection

namespace ramrod {
  namespace gui {
    class input : public gui::element
    {
    public:
      input(const std::string &initial_text,
            const float width, const float height, const float position_x,
            const float position_y, const std::size_t tab_index);
      /**
       * @brief Loses the focus for this element
       */
      void blur();
      /**
       * @brief Clearing all the characters
       *
       * @return `true` if there was content
       */
      bool clear();
      /**
       * @brief Focuses this element
       *
       * @param select_all `true` to select all its content
       */
      void focus(const bool select_all = true);
      /**
       * @brief Inserting text into this element
       *
       * @param text Text that will be inserted at the current selected position
       *
       * @return `false`if the full text with the the new insertion is larger than
       * the maximum allowed
       */
      bool insert(const std::string &text);
      /**
       * @brief Getting the maximum number of characters that should fit this element
       *
       * @return Maximum number of characters for this element
       */
      std::size_t maximum_characters();
      /**
       * @brief Setting a new maximum number of characters for this element
       *
       * @param new_maximum New maximum number of charactes' value
       */
      void maximum_characters(const std::size_t new_maximum);
      /**
       * @brief Selecting text
       *
       * @param start Starting character's index to be selected
       * @param count Number of characters to select
       *
       * @return The number of selected characters
       */
      std::size_t select(const std::size_t start, const std::size_t count);
      /**
       * @brief Selecting all the text in this element
       *
       * @return The number of selected characters
       */
      std::size_t select_all();
      /**
       * @brief Getting the selected text
       *
       * @return A string containing all the selected text
       */
      std::string selection();
      /**
       * @brief Getting the total number of characters in this text input
       *
       * @return Number of characters in this text input
       */
      std::size_t total();
      /**
       * @brief Getting all the text inside this text input
       *
       * @return All the text inside
       */
      const std::string &value();
      /**
       * @brief Changing the whole text
       *
       * @param new_text The new text
       *
       * @return `false` if string is larger than the maximum allowed
       */
      bool value(const std::string &new_text);

    protected:
      /**
       * @brief Setting the background color
       *
       * @param red   Red's color value from 0 to 255
       * @param green Green's color value from 0 to 255
       * @param blue  Blue's color value from 0 to 255
       * @param alpha Transparency's color value from 0 to 255
       */
      virtual void background_color(const std::uint8_t red,
                                    const std::uint8_t green,
                                    const std::uint8_t blue,
                                    const std::uint8_t alpha);
      /**
       * @brief Setting the background color
       *
       * @param red   Red's color value from 0.0f to 1.0f
       * @param green Green's color value from 0.0f to 1.0f
       * @param blue  Blue's color value from 0.0f to 1.0f
       * @param alpha Transparency's color value from 0.0f to 1.0f
       */
      virtual void background_color(const float red,
                                    const float green,
                                    const float blue,
                                    const float alpha);
      /**
       * @brief Setting the border color
       *
       * @param red   Red's color value from 0 to 255
       * @param green Green's color value from 0 to 255
       * @param blue  Blue's color value from 0 to 255
       * @param alpha Transparency's color value from 0 to 255
       */
      virtual void border_color(const std::uint8_t red,
                                const std::uint8_t green,
                                const std::uint8_t blue,
                                const std::uint8_t alpha);
      /**
       * @brief Setting the border color
       *
       * @param red   Red's color value from 0.0f to 1.0f
       * @param green Green's color value from 0.0f to 1.0f
       * @param blue  Blue's color value from 0.0f to 1.0f
       * @param alpha Transparency's color value from 0.0f to 1.0f
       */
      virtual void border_color(const float red,
                                const float green,
                                const float blue,
                                const float alpha);
      /**
       * @brief Defining the border width
       *
       * @param width Floating value for border width
       */
      virtual void border_width(const float width);
      /**
       * @brief Setting the text color
       *
       * @param red   Red's color value from 0 to 255
       * @param green Green's color value from 0 to 255
       * @param blue  Blue's color value from 0 to 255
       * @param alpha Transparency's color value from 0 to 255
       */
      virtual void text_color(const std::uint8_t red,
                              const std::uint8_t green,
                              const std::uint8_t blue,
                              const std::uint8_t alpha);
      /**
       * @brief Setting the text color
       *
       * @param red   Red's color value from 0.0f to 1.0f
       * @param green Green's color value from 0.0f to 1.0f
       * @param blue  Blue's color value from 0.0f to 1.0f
       * @param alpha Transparency's color value from 0.0f to 1.0f
       */
      virtual void text_color(const float red,
                              const float green,
                              const float blue,
                              const float alpha);
      /**
       * @brief Changed the text size in pixels
       *
       * @param pixels New size in pixels
       */
      virtual void text_size(const std::uint16_t pixels);
      /**
       * @brief Changing the text size in em
       *
       * @param size New size in em
       */
      virtual void text_size(const float size);

      virtual void paint();

      /*
      virtual void blur_event(const gui::event::focus &event);
      virtual void focus_event(const gui::event::focus &event);
      // Keyboard events
      virtual void key_down_event(const gui::event::keyboard &event);
      virtual void key_up_event(const gui::event::keyboard &event);
      // Mouse events
      virtual void click_event(const gui::event::mouse &event);
      virtual void double_click_event(const gui::event::mouse &event);
      virtual void mouse_down_event(const gui::event::mouse &event);
      virtual void mouse_enter_event(const gui::event::mouse &event);
      virtual void mouse_leave_event(const gui::event::mouse &event);
      virtual void mouse_move_event(const gui::event::mouse &event);
      virtual void mouse_over_event(const gui::event::mouse &event);
      virtual void mouse_out_event(const gui::event::mouse &event);
      virtual void mouse_up_event(const gui::event::mouse &event);
      virtual void wheel_event(const gui::event::wheel &event);
      // Drag and drop events TODO: decide it those are necessary
      virtual void drag_event();
      virtual void drag_end_event();
      virtual void drag_enter_event();
      virtual void drag_start_event();
      virtual void drag_leave_event();
      virtual void drag_over_event();
      virtual void drop_event();
      // Storage events
      virtual void change_event();
      virtual void copy_event(const gui::event::clipboard &event);
      virtual void cut_event(const gui::event::clipboard &event);
      virtual void paste_event(const gui::event::clipboard &event);
      virtual void select_event();
      // Size events
      virtual void resize_event(const gui::event::resize &event);
      */

      std::string characters_;
      std::size_t character_position_;
      std::size_t character_count_;
      std::size_t max_characters_;
      bool active_;

      gui::selection selection_;

    private:
      ramrod::gl::buffer text_buffer_;
      float text_size_;

      inline static unsigned int u_text_size_ = 0;
    };
  } // namespace: gui
} // namespace: ramrod

#endif // RAMROD_GUI_GUI_MANAGER_H
