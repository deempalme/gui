#ifndef RAMROD_VISION_GUI_INPUT_TEXT_H
#define RAMROD_VISION_GUI_INPUT_TEXT_H

#include <iosfwd>              // for size_t
#include <string>              // for string, allocator

#include "ramrod/gl/buffer.h"  // for buffer
#include "ramrod/gui/input.h"  // for input
#include "ramrod/gui/types.h"  // for position, size, selection

namespace ramrod {
  namespace gui {
    class window;

    class input_text : public gui::input
    {
    public:
      explicit input_text(gui::window *window, const std::string &initial_text = "",
                          const float width = 150.0f, const float height = 35.0f,
                          const float position_x = 0, const float position_y = 0,
                          const int z_index = 0, const std::size_t tab_index = 0);

    private:
      void update_buffer();

      ramrod::gl::buffer background_buffer_;
    };
  } // namespace: gui
} // namespace: ramrod

#endif // RAMROD_VISION_GUI_INPUT_TEXT_H
