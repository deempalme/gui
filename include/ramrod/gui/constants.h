#ifndef RAMROD_GUI_CONSTANTS_H
#define RAMROD_GUI_CONSTANTS_H

#include <cstdint>

namespace ramrod {
  namespace gui {
    namespace byte_sizes {
      constexpr int float_1D = sizeof(float);
      constexpr int float_2D = float_1D * 2;
      constexpr int float_3D = float_1D * 3;
      constexpr int float_4D = float_1D * 4;
    } // namespace: byte_sizes

    namespace identifiers {
      constexpr float max_identifier_count = 50000.0f;
    } // namespace: identifiers

    namespace sprite {
      constexpr std::uint32_t width  = 512;
      constexpr std::uint32_t height = 512;

      constexpr std::uint32_t input_text_u_1    = 8;
      constexpr std::uint32_t input_text_u_2    = 72;
      constexpr std::uint32_t input_text_v_1    = 8;
      constexpr std::uint32_t input_text_v_2    = 72;
      constexpr std::uint32_t input_text_border = 5;
    } // namespace: sprite

    constexpr float float_max_range = 0.0005f;

    constexpr bool equal_floats(const float a, const float b){
      return (a + float_max_range) > b && (a - float_max_range) < b;
    }
    // GUI's sprite image
    constexpr char default_sprite_path[] = RAMROD_GUI_DIR "/resources/gui/sprite.png";
    // Default fonts
    constexpr char rubik_regular_image[] = RAMROD_GUI_DIR "/resources/fonts/rubik_regular.png";
    constexpr char rubik_regular_info[]  = RAMROD_GUI_DIR "/resources/fonts/rubik_regular.fnt";
    constexpr char rubik_medium_image[]  = RAMROD_GUI_DIR "/resources/fonts/rubik_medium.png";
    constexpr char rubik_medium_info[]   = RAMROD_GUI_DIR "/resources/fonts/rubik_medium.fnt";
    // OpenGL shaders
    constexpr char text_shader_frag[]   = RAMROD_GUI_DIR "/resources/shaders/simple_text.frag";
    constexpr char text_shader_geom[]   = RAMROD_GUI_DIR "/resources/shaders/simple_text.geom";
    constexpr char text_shader_vert[]   = RAMROD_GUI_DIR "/resources/shaders/simple_text.vert";
    constexpr char sprite_shader_frag[] = RAMROD_GUI_DIR "/resources/shaders/gui_sprite.frag";
    constexpr char sprite_shader_vert[] = RAMROD_GUI_DIR "/resources/shaders/gui_sprite.vert";

    namespace uniform_buffer {
      constexpr int font_size_bytes     = gui::byte_sizes::float_1D;
      constexpr int font_size_offset    = gui::byte_sizes::float_2D;
      constexpr int window_binding_size = gui::byte_sizes::float_4D;
      constexpr int window_size_bytes   = gui::byte_sizes::float_2D;
      constexpr int window_size_offset  = 0;
    } // namespace: uniform_buffer
  } // namespace: gui
} // namespace: ramrod

#endif // RAMROD_GUI_ENUMERATORS_H
