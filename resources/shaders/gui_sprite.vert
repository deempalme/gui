#version 420 core

layout(location = 0) in vec2 i_position;
layout(location = 1) in vec2 i_texture;

out vec2 f_texture;

layout (binding = 1, std140) uniform u_window {
  vec2  u_window_size;
  float u_font_size;
};

void main(void){
  f_texture = i_texture;
  const float del_x = u_window_size.x <= 0 ? 512 : u_window_size.x;
  const float del_y = u_window_size.y <= 0 ? 256 : u_window_size.y;
  gl_Position = vec4(i_position.x / del_x * 2.0 - 1.0,
                    -i_position.y / del_y * 2.0 + 1.0, 0.0, 1.0);
//  gl_Position = vec4(i_position.x * 2.0 - 1.0, -i_position.y * 2.0 + 1.0, 0.0, 1.0);
}
