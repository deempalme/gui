#ifndef RAMROD_GUI_ENUMERATORS_H
#define RAMROD_GUI_ENUMERATORS_H

namespace ramrod {
  namespace gui {
    namespace attribute {
      enum index : int {
        position = 0,
        texture  = 1,
        size     = 2,
        color    = 2,
        offset   = 3,
        id       = 4
      };
    } // namespace: attribute

    namespace battery {
      enum power_state : int {
        error      = -1,
        unknown    = 0,
        on_battery = 1,
        no_battery = 2,
        charging   = 3,
        charged    = 4
      };
    } // namespace: battery

    namespace error {
      enum types : int {
        window_closed     =  0,
        SDL2_not_loaded   = -1,
        window_not_loaded = -2,
        GLAD_not_loaded   = -3,
        existing_window   = -4,
        OpenGL_not_loaded = -5
      };
    } // namespace: error

    namespace framebuffer {
      enum attatchment : int {
        front = 0,
        back  = 1
      };
    } // namespace: framebuffer

    namespace keyboard {
      enum key_mod : int {
        none               = 0,
        ctrl               = 0x00c0,
        shift              = 0x0003,
        alt                = 0x0300,
        gui                = 0x0c00,
        ctrl_shift         = 0x00c3,
        ctrl_alt           = 0x03c0,
        ctrl_gui           = 0x0cc0,
        shift_alt          = 0x0303,
        shift_gui          = 0x0c03,
        alt_gui            = 0x0f00,
        ctrl_shift_alt     = 0x03c3,
        ctrl_shift_gui     = 0x0cc3,
        ctrl_alt_gui       = 0x0fc0,
        shift_alt_gui      = 0x0f03,
        ctrl_shift_alt_gui = 0x0fc3,
        num                = 0x1000,
        caps               = 0x2000,
        node               = 0x4000,
        reserved           = 0x8000,
        not_none           = 0x0fff
      };
    } // namespace: keyboard

    namespace texture_unit {
      enum id : int {
        albedo         = 0,
        sprite         = 1,
        font_atlas     = 2,
        front_frame    = 3,
        background_ids = 4,
        video          = 5,
        image_write    = 6
      };
    } // namespace: texture

    namespace uniform_buffer {
      enum attatchment : unsigned int {
        scene = 1
      };
    } // namespace: framebuffer

    namespace vector_size {
      enum size : int {
        vector_1D = 1,
        vector_2D = 2,
        vector_3D = 3,
        vector_4D = 4
      };
    } // namespace: vector_size
  } // namespace: gui
} // namespace: ramrod

#endif // RAMROD_GUI_ENUMERATORS_H
