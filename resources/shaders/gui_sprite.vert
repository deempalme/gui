#version 420 core

layout(location = 0) in vec2 i_position;
layout(location = 1) in vec2 i_texture;

out vec2 f_texture;

layout (std140, binding = 1) uniform u_window {
  vec2  u_window_size;
  float u_font_size;
};

void main(void){
  f_texture = i_texture;
  gl_Position = vec4(i_position / u_window_size * 2.0 - 1.0, 0.0, 1.0);
}
