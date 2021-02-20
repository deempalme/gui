#include "ramrod/gui/slider.h"

namespace ramrod {
  namespace gui {
    slider::slider(const float min_value, const float max_value, const float initial_value,
                   const float width, const float height,
                   const float position_x, const float position_y,
                   const int z_index, const std::size_t tab_index)

    {

    }

    float slider::max_value(){
      return max_value_;
    }

    void slider::max_value(const float new_maximum_value){
      max_value_ = new_maximum_value;
    }

    float slider::min_value(){
      return min_value_;
    }

    void slider::min_value(const float new_minimum_value){
      min_value_ = new_minimum_value;
    }

    float slider::slider_position(){
      return current_value_;
    }

    bool slider::slider_position(const float new_position){
      if(current_value_ < min_value_ || current_value_ > max_value_) return false;
      current_value_ = new_position;
      return true;
    }
  } // namespace: gui
} // namespace: ramrod

