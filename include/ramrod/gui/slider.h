#ifndef RAMROD_GUI_SLIDER_H
#define RAMROD_GUI_SLIDER_H

#include <cstdint>

namespace ramrod {
  namespace gui {
    class slider
    {
    public:
      slider(const float min_value = 0,
             const float max_value = 100,
             const float initial_value = 50,
             const float width = 150.0f, const float height = 35.0f,
             const float position_x = 0, const float position_y = 0,
             const int z_index = 0, const std::size_t tab_index = 0);

      float max_value();
      void max_value(const float new_maximum_value);

      float min_value();
      void min_value(const float new_minimum_value);

      float slider_position();
      bool slider_position(const float new_position);

    protected:

    private:
      float current_value_;
      float min_value_;
      float max_value_;
    };
  } // namespace: gui
} // namespace: ramrod

#endif // RAMROD_GUI_SLIDER_H
