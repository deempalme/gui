#ifndef RAMROD_GUI_TYPE_EVENTS_H
#define RAMROD_GUI_TYPE_EVENTS_H

#include <cstdint>

#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_scancode.h>

namespace ramrod::gui {
  namespace finger_event {
    typedef struct finger {
      std::uint32_t timestamp;  // In milliseconds, populated using SDL_GetTicks()
      std::uint32_t window_id;  // The window underneath the finger, if any
      std::int64_t touch_id;    // The touch device id
      std::int64_t finger_id;   // The finger id
      float x;                  // Normalized in the range  0...1
      float y;                  // Normalized in the range  0...1
      float dx;                 // Normalized in the range -1...1
      float dy;                 // Normalized in the range -1...1
      float pressure;           // Normalized in the range  0...1
    } finger;
  } // namespace: finger_event

  namespace keyboard_event {
    typedef struct key {
      std::uint32_t timestamp;  // In milliseconds, populated using SDL_GetTicks()
      std::uint32_t windowID;   // The window with keyboard focus, if any
      std::uint8_t repeat;      // Non-zero if this is a key repeat
      SDL_Scancode scancode;    // SDL physical key code - see ::SDL_Scancode for details
      SDL_Keycode  sym;         // SDL virtual key code - see ::SDL_Keycode for details
      std::uint16_t mod;        // current key modifiers
    } key;
  } // namespace: keyboard_event

  namespace mouse_event {
    typedef struct button {
      std::uint32_t timestamp;  // In milliseconds, populated using SDL_GetTicks()
      std::uint32_t window_id;  // The window with mouse focus, if any
      std::uint32_t which;      // The mouse instance id, or SDL_TOUCH_MOUSEID
      std::uint8_t button;      // The mouse button index
      std::uint8_t clicks;      // 1 for single-click, 2 for double-click, etc.
      std::int32_t x;           // X coordinate, relative to window
      std::int32_t y;           // Y coordinate, relative to window
    } button;

    typedef struct move {
      std::uint32_t timestamp;  // In milliseconds, populated using SDL_GetTicks()
      std::uint32_t window_id;  // The window with mouse focus, if any
      std::uint32_t which;      // The mouse instance id, or SDL_TOUCH_MOUSEID
      std::uint32_t state;      // The current button state
      std::int32_t x;           // X coordinate, relative to window
      std::int32_t y;           // Y coordinate, relative to window
      std::int32_t dx;          // The relative motion in the X direction
      std::int32_t dy;          // The relative motion in the Y direction
    } move;

    typedef struct wheel {
      std::uint32_t timestamp;  // In milliseconds, populated using SDL_GetTicks()
      std::uint32_t window_id;  // The window with mouse focus, if any
      std::uint32_t which;      // The mouse instance id, or SDL_TOUCH_MOUSEID
      std::int32_t x;           // The amount scrolled horizontally, positive to the right
                                // and negative to the left
      std::int32_t y;           // The amount scrolled vertically, positive away from the user
                                // and negative toward the user
      std::uint32_t direction;  // Set to one of the SDL_MOUSEWHEEL_* defines. When FLIPPED
                                // the values in X and Y will be opposite. Multiply by -1 to
                                // change them back
    } wheel;
  } // namespace: mouse_event

  namespace multigesture {
    typedef struct event {
      std::uint32_t timestamp;    // Timestamp of the event
      std::int64_t touch_id;      // The touch device id
      float d_theta;              // The amount that the fingers rotated during this motion
      float d_dist;               // The amount that the fingers pinched during this motion
      float x;                    // The normalized center of gesture
      float y;                    // The normalized center of gesture
      std::uint16_t num_fingers;  // The number of fingers used in the gesture
      std::uint16_t padding;      // Padding
    } event;
  }

  namespace window_event {
    typedef struct move {
      std::uint32_t timestamp;  // In milliseconds, populated using SDL_GetTicks()
      std::uint32_t window_id;  // The associated window
      std::int32_t x;           // event dependent data
      std::int32_t y;           // event dependent data
    } move;

    typedef struct resize {
      std::uint32_t timestamp;  // In milliseconds, populated using SDL_GetTicks()
      std::uint32_t window_id;  // The associated window
      std::int32_t width;       // event dependent data
      std::int32_t height;      // event dependent data
    } resize;

    typedef struct window {
      std::uint32_t timestamp;  // In milliseconds, populated using SDL_GetTicks()
      std::uint32_t window_id;  // The associated window
    } window;
  } // namespace: window_event
} // namespace ramrod::gui


#endif // RAMROD_GUI_TYPE_EVENTS_H
