#ifndef RAMROD_VISION_SERVER_GUI_INPUT_TEXT_H
#define RAMROD_VISION_SERVER_GUI_INPUT_TEXT_H

#include <iosfwd>              // for size_t
#include <string>              // for string, allocator

#include "ramrod/gl/buffer.h"  // for buffer
#include "ramrod/gui/input.h"  // for input
#include "ramrod/gui/types.h"  // for position, size, selection

namespace ramrod {
  namespace gui {
    class input_text : public gui::input
    {
    public:
      /**
       * @brief Input text's contructor
       *
       * @param initial_text Defines the text that this will have since the beginning
       * @param width This element's width in pixels
       * @param height This element's height in pixels
       * @param position_x This element's horizontal position in pixels from left
       * @param position_y This element's vertical position in pixels from top
       * @param tab_index The tab index value for this input, leave it 0 to autodefine it
       */
      explicit input_text(const std::string &initial_text = "",
                          const float width = 150.0f, const float height = 35.0f,
                          const float position_x = 0, const float position_y = 0,
                          const std::size_t tab_index = 0);
      virtual ~input_text();

      virtual void paint();

    private:
      void change_size();

      ramrod::gl::buffer background_buffer_;
    };
  } // namespace: gui
} // namespace: ramrod

#endif // RAMROD_VISION_SERVER_GUI_INPUT_TEXT_H
