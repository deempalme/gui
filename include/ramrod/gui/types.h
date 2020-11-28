#ifndef RAMROD_VISION_SERVER_GUI_TYPES_H
#define RAMROD_VISION_SERVER_GUI_TYPES_H

#include <cstddef>

#include "ramrod/gui/enumerators.h"

namespace ramrod {
  namespace gui {
    template<typename T>
    struct point_4d {
      T x;
      T y;
      T z;
      T w;
    };

    template<typename T>
    struct position {
      T x = 0;
      T y = 0;
    };

    template<typename T>
    union pixel {
      struct color {
        T b;
        T g;
        T r;
        T a;
      } color;
      T data[4];
    };

    typedef struct identifier {
      std::size_t element;
      std::size_t children;
      bool pointing_next;
    } identifier;

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

    namespace battery {
      typedef struct status {
        gui::battery::power_state status;
        // -1 if it cannot be determined or not running on battery
        int seconds_left;
        // -1 if it cannot be determined or not running on battery
        int percentage_left;
      } status;
    } // namespace: battery
  } // namespace: gui
} // namespace: ramrod


#endif // RAMROD_VISION_SERVER_GUI_TYPES_H
