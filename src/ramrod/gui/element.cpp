#include "ramrod/gui/element.h"

#include "ramrod/gui/constants.h"
#include "ramrod/gui/gui_manager.h"

namespace ramrod {
  namespace gui {
    element::element(const float width, const float height,
                     const float position_x, const float position_y,
                     const std::size_t tab_index) :
      id_{gui::gui_manager::last_element_id() + 1},
      tab_index_{tab_index},
      position_{position_x, position_y},
      size_{width, height},
      update_{false},
      resize_{false}
    {}

    std::size_t element::id(){
      return id_;
    }

    gui::position<float> element::position(){
      return position_;
    }

    void element::position(const float x, const float y){
      position_.x = x;
      position_.y = y;
      resize_ = true;;
      update_ = true;
    }

    gui::size<float> element::size(){
      return size_;
    }

    bool element::size(const float width, const float height){
      if((width <= 0 && height <= 0)
      || (gui::equal_floats(size_.width, width) && gui::equal_floats(size_.height, height)))
        return false;

      size_.width = width;
      size_.height = height;
      resize_ = true;
      return update_ = true;
    }

    std::size_t element::tab_index(){
      return tab_index_;
    }

    void element::tab_index(const std::size_t new_tab_index){
      tab_index_ = new_tab_index;
    }

    // :::::::::::::::::::::::::::::::::::: PROTECTED FUNCTIONS :::::::::::::::::::::::::::::::::::

    void element::change_size(){
      resize_ = false;
    }
  } // namespace: gui
} // namespace: ramrod
