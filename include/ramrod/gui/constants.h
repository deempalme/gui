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
      constexpr int float_5D = float_1D * 5;
      constexpr int float_7D = float_1D * 7;
      constexpr int float_8D = float_1D * 8;
      constexpr int float_9D = float_1D * 9;
      constexpr int float_11D = float_1D * 11;
      constexpr int float_12D = float_1D * 12;
      constexpr int float_14D = float_1D * 14;
      constexpr int float_15D = float_1D * 15;
    } // namespace: byte_sizes

    namespace resolution {
      constexpr unsigned int full_hd_width  = 1920;
      constexpr unsigned int full_hd_height = 1080;
      constexpr unsigned int full_hd = 1920 * 1080;
    } // namespace: resolution

    namespace shader {
      constexpr char frame_shader_frag[] = RAMROD_GUI_DIR "/resources/shaders/frame_shader.frag";
      constexpr char frame_shader_vert[] = RAMROD_GUI_DIR "/resources/shaders/frame_shader.vert";

      constexpr char sprite_shader_frag[] = RAMROD_GUI_DIR "/resources/shaders/sprite_shader.frag";
      constexpr char sprite_shader_geom[] = RAMROD_GUI_DIR "/resources/shaders/sprite_shader.geom";
      constexpr char sprite_shader_vert[] = RAMROD_GUI_DIR "/resources/shaders/sprite_shader.vert";

      constexpr char text_shader_frag[] = RAMROD_GUI_DIR "/resources/shaders/simple_text.frag";
      constexpr char text_shader_geom[] = RAMROD_GUI_DIR "/resources/shaders/simple_text.geom";
      constexpr char text_shader_vert[] = RAMROD_GUI_DIR "/resources/shaders/simple_text.vert";

      constexpr char yuyv_frag_shader[] = RAMROD_GUI_DIR "/resources/shaders/yuyv.frag";
    }

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

  } // namespace: gui
} // namespace: ramrod

#endif // RAMROD_GUI_ENUMERATORS_H
