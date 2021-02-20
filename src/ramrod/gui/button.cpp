#include "ramrod/gui/button.h"

#include "ramrod/gui/constants.h"    // for float_4D, float_2D
#include "ramrod/gui/enumerators.h"  // for position, texture

namespace ramrod {
  namespace gui {
    button::button(gui::window *window, const std::string &initial_text,
                   const float width, const float height,
                   const float position_x, const float position_y,
                   const int z_index, const std::size_t tab_index) :
      gui::element(window, width, height, position_x, position_y, z_index, tab_index),
      text_(initial_text)
    {}

    // ::::::::::::::::::::::::::::::::::::: PRIVATE FUNCTIONS ::::::::::::::::::::::::::::::::::::

  } // namespace: gui
} // namespace: ramrod
