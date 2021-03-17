#include "ramrod/gui/image.h"

#include "ramrod/gui/constants.h"

namespace ramrod {
  namespace gui {
    image::image(gui::window *window, const float width, const float height,
                 const float position_x, const float position_y,
                 const int z_index, const std::size_t tab_index) :
      gui::element(window, width, height, position_x, position_y, z_index, tab_index)
    {}

    // ::::::::::::::::::::::::::::::::::: PROTECTED FUNCTIONS :::::::::::::::::::::::::::::::::::

    // :::::::::::::::::::::::::::::::::::: PRIVATE FUNCTIONS ::::::::::::::::::::::::::::::::::::

  } // namespace: gui
} // namespace: ramrod

