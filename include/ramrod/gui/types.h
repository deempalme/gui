#ifndef RAMROD_VISION_SERVER_GUI_TYPES_H
#define RAMROD_VISION_SERVER_GUI_TYPES_H

#include <cstdint>

#include "ramrod/gui/enumerators.h"

namespace ramrod {
  namespace gui {
    class element;

    namespace battery {
      typedef struct status {
        gui::battery::power_state status;
        // -1 if it cannot be determined or not running on battery
        int seconds_left;
        // -1 if it cannot be determined or not running on battery
        int percentage_left;
      } status;
    } // namespace: battery

    typedef struct pixel_id {
      std::size_t parent     = 0;
      std::size_t object_id  = 0;
      gui::element *object   = nullptr;
      std::uint32_t object_x = 0;
      std::uint32_t object_y = 0;
    } pixel_id;

    template<typename T>
    struct point_2d {
      T x;
      T y;
    };

    template<typename T>
    struct point_4d {
      T x;
      T y;
      T z;
      T w;
    };

    typedef struct point_xy_id_uv {
      float x;
      float y;
      float id;
      float u;
      float v;
    } point_2d_uv;

    template<typename T>
    struct position {
      T x = 0;
      T y = 0;
    };

    typedef struct selection {
      std::size_t start = 0;
      std::size_t count = 0;
    } selection;

    template<typename T>
    struct size {
      T width = 0;
      T height = 0;
    };

    namespace text {
      typedef struct character {
        union texture {
          struct map {
            // Dimensions
            float width;
            float height;
            // Texture
            float u1;
            float v1;
            float u2;
            float v2;
          } map;
          float data[6] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
        } texture;
        union position {
          struct offset {
            // Offset
            float x;
            float y;
            // Next character position
            float next;
          } offset;
          float data[3] = { 0.0f, 0.0f, 0.0f };
        } position;
      } character;
    } // namespace: text

    typedef struct texture_uv {
      float u1;
      float v1;
      float u2;
      float v2;
    } texture_uv;

  } // namespace: gui
} // namespace: ramrod


#endif // RAMROD_VISION_SERVER_GUI_TYPES_H
