#ifndef RAMROD_GUI_IMAGE_H
#define RAMROD_GUI_IMAGE_H

#include "ramrod/gui/element.h"

namespace ramrod {
  namespace gui {
    class window;

    class image : public gui::element
    {
    public:
      image(gui::window *window, const float width = 128, const float height = 128,
            const float position_x = 0, const float position_y = 0);

    protected:

    private:

    };
  } // namespace: gui
} // namespace: ramrod

#endif // RAMROD_GUI_IMAGE_H
