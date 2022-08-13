#ifndef RAMROD_EVENT_H
#define RAMROD_EVENT_H

#include <cstdint>

#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_scancode.h>

namespace ramrod::event {
  typedef struct finger {
    std::uint32_t timestamp;  // In milliseconds, populated using SDL_GetTicks()
    std::uint32_t window_id;  // The window underneath the finger, if any
    std::int64_t  touch_id;   // The touch device id
    std::int64_t  finger_id;  // The finger id
    float x;                  // Normalized in the range  0...1
    float y;                  // Normalized in the range  0...1
    float dx;                 // Normalized in the range -1...1
    float dy;                 // Normalized in the range -1...1
    float pressure;           // Normalized in the range  0...1
  } finger;

  typedef struct key {
    std::uint32_t timestamp;  // In milliseconds, populated using SDL_GetTicks()
    std::uint32_t window_id;  // The window with keyboard focus, if any
    std::uint8_t  repeat;     // Non-zero if this is a key repeat
    SDL_Scancode  scancode;   // SDL physical key code - see ::SDL_Scancode for details
    SDL_Keycode   sym;        // SDL virtual key code - see ::SDL_Keycode for details
    std::uint16_t mod;        // current key modifiers
  } key;

  typedef struct mouse_button {
    std::uint32_t timestamp;  // In milliseconds, populated using SDL_GetTicks()
    std::uint32_t window_id;  // The window with mouse focus, if any
    std::uint32_t which;      // The mouse instance id, or SDL_TOUCH_MOUSEID
    std::uint8_t  button;     // The mouse button index
    std::uint8_t  clicks;     // 1 for single-click, 2 for double-click, etc.
    std::int32_t  x;          // X coordinate, relative to window
    std::int32_t  y;          // Y coordinate, relative to window
  } mouse_button;

  typedef struct mouse_move {
    std::uint32_t timestamp;  // In milliseconds, populated using SDL_GetTicks()
    std::uint32_t window_id;  // The window with mouse focus, if any
    std::uint32_t which;      // The mouse instance id, or SDL_TOUCH_MOUSEID
    std::uint32_t state;      // The current button state
    std::int32_t  x;          // X coordinate, relative to window
    std::int32_t  y;          // Y coordinate, relative to window
    std::int32_t  dx;         // The relative motion in the X direction
    std::int32_t  dy;         // The relative motion in the Y direction
  } mouse_move;

  typedef struct mouse_wheel {
    std::uint32_t timestamp;  // In milliseconds, populated using SDL_GetTicks()
    std::uint32_t window_id;  // The window with mouse focus, if any
    std::uint32_t which;      // The mouse instance id, or SDL_TOUCH_MOUSEID
    std::int32_t  x;          // The amount scrolled horizontally, positive to the right
    // and negative to the left
    std::int32_t  y;          // The amount scrolled vertically, positive away from the user
    // and negative toward the user
    std::uint32_t direction;  // Set to one of the SDL_MOUSEWHEEL_* defines. When FLIPPED
    // the values in X and Y will be opposite. Multiply by -1 to
    // change them back
  } mouse_wheel;

  typedef struct multigesture {
    std::uint32_t timestamp;    // Timestamp of the event
    std::int64_t  touch_id;     // The touch device id
    std::uint16_t num_fingers;  // The number of fingers used in the gesture
    std::uint16_t padding;      // Padding
    float d_theta;              // The amount that the fingers rotated during this motion
    float d_dist;               // The amount that the fingers pinched during this motion
    float x;                    // The normalized center of gesture
    float y;                    // The normalized center of gesture
  } multigesture;

  typedef struct window_xy {
    std::uint32_t timestamp;  // In milliseconds, populated using SDL_GetTicks()
    std::uint32_t window_id;  // The associated window
    std::int32_t  x;          // New X position
    std::int32_t  y;          // New Y position
  } window_hide, window_move, window_show, window;

  typedef struct window_resize {
    std::uint32_t timestamp;  // In milliseconds, populated using SDL_GetTicks()
    std::uint32_t window_id;  // The associated window
    std::int32_t  width;      // New window width
    std::int32_t  height;     // New window height
  } window_resize;
} // namespace ramrod::event


#endif // RAMROD_EVENT_H
