#include "ramrod/gui/image.h"

#include "ramrod/gui/constants.h"

namespace ramrod {
  namespace gui {
    image::image(gui::window *window, const float width, const float height,
                 const float position_x, const float position_y) :
      gui::element(window, width, height, position_x, position_y, 0)
    {}

    // ::::::::::::::::::::::::::::::::::: PROTECTED FUNCTIONS :::::::::::::::::::::::::::::::::::

    // :::::::::::::::::::::::::::::::::::: PRIVATE FUNCTIONS ::::::::::::::::::::::::::::::::::::

  } // namespace: gui
} // namespace: ramrod

