#include "ramrod/gui/time_slider.h"

#include "ramrod/gui/window.h"

namespace ramrod {
  namespace gui {
    time_slider::time_slider(gui::window *window, const std::int64_t initial_time,
                             const std::int64_t start_time, const std::int64_t end_time,
                             const float width, const float height,
                             const float position_x, const float position_y,
                             const int z_index, const std::size_t tab_index) :
      gui::element(window, width, height, position_x, position_y, z_index, tab_index),
      spot_(window, width, height, position_x, position_y, z_index, tab_index),
      start_time_{start_time},
      end_time_{end_time},
      current_time_{initial_time}
    {}

    std::int64_t time_slider::end_time(){
      return end_time_;
    }

    void time_slider::end_time(const std::int64_t new_end_time){
      end_time_ = new_end_time;
    }

    std::int64_t time_slider::start_time(){
      return start_time_;
    }

    void time_slider::start_time(const std::int64_t new_start_time){
      start_time_ = new_start_time;
    }

    std::int64_t time_slider::time_position(){
      return current_time_;
    }

    bool time_slider::time_position(const std::int64_t new_position){
      if(current_time_ < start_time_ || current_time_ > end_time_) return false;
      current_time_ = new_position;
      return true;
    }
  } // namespace: gui
} // namespace: ramrod

