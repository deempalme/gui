#ifndef RAMROD_GUI_ELEMENT_H
#define RAMROD_GUI_ELEMENT_H

#include <cstdint>

#include "ramrod/gui/types.h"  // for position, size, selection

namespace ramrod {
  namespace gui {
    class element
    {
    public:
      element(const float width, const float height,
              const float position_x, const float position_y,
              const std::size_t tab_index);
      /**
       * @brief Getting this element's id
       *
       * @return Numeric identifier for this element
       */
      std::size_t id();
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
       * @brief Getting the current tab index of this element
       *
       * @return Tab index number
       */
      std::size_t tab_index();
      /**
       * @brief Setting a new tab index number for this element
       *
       * @param new_tab_index New tab index number
       */
      void tab_index(const std::size_t new_tab_index);

    protected:
      virtual void change_size();

      std::size_t id_;
      std::size_t tab_index_;
      gui::position<float> position_;
      gui::size<float> size_;
      bool update_, resize_;

    };
  } // namespace: gui
} // namespace: ramrod

#endif // RAMROD_GUI_ELEMENT_H
