#include "ramrod/gui/element.h"

#include "ramrod/gui/constants.h"
#include "ramrod/gui/gui_manager.h"

namespace ramrod {
  namespace gui {
    element::element(const float width, const float height,
                     const float position_x, const float position_y) :
      id_{gui::gui_manager::insert_new_element(this)},
      position_{position_x, position_y},
      size_{width, height},
      update_{false},
      resize_{false},
      hidden_{false}
    {
    }

    void element::hide(const bool hide){
      hidden_ = hide;
    }

    bool element::hidden(){
      return hidden_;
    }

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

    void element::paint(){}

    // :::::::::::::::::::::::::::::::::::: PROTECTED FUNCTIONS :::::::::::::::::::::::::::::::::::

    void element::change_size(){
      resize_ = false;
    }
  } // namespace: gui
} // namespace: ramrod
