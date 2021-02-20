#ifndef RAMROD_GUI_BUTTON_H
#define RAMROD_GUI_BUTTON_H

#include <string>

#include "ramrod/gui/element.h"

namespace ramrod {
  namespace gui {
    class window;

    class button : public gui::element
    {
    public:
      button(gui::window *window, const std::string &initial_text = "button",
             const float width = 150.0f, const float height = 35.0f,
             const float position_x = 0, const float position_y = 0,
             const int z_index = 0, const std::size_t tab_index = 0);

    protected:

    private:
      std::string text_;
    };
  } // namespace: gui
} // namespace: ramrod

#endif // RAMROD_GUI_BUTTON_H
