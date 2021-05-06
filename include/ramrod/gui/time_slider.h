#ifndef RAMROD_GUI_TIME_SLIDER_H
#define RAMROD_GUI_TIME_SLIDER_H

#include <cstdint>

#include "ramrod/gui/image.h"

namespace ramrod {
  namespace gui {
    class window;

    class time_slider : protected gui::element
    {
    public:
      time_slider(gui::window *window, const std::int64_t initial_time,
                  const std::int64_t start_time, const std::int64_t end_time,
                  const float width = 150.0f, const float height = 35.0f,
                  const float position_x = 0, const float position_y = 0,
                  const int z_index = 0, const std::size_t tab_index = 0);

      std::int64_t end_time();
      void end_time(const std::int64_t new_end_time);

      std::int64_t start_time();
      void start_time(const std::int64_t new_start_time);

      std::int64_t time_position();
      bool time_position(const std::int64_t new_position);

    protected:

    private:
      gui::image spot_;

      std::int64_t start_time_;
      std::int64_t end_time_;
      std::int64_t current_time_;
    };
  } // namespace: gui
} // namespace: ramrod

#endif // RAMROD_GUI_TIME_SLIDER_H
