#version 420 core

layout(location = 0) in vec2 i_position;
layout(location = 1) in vec2 i_texture;

out vec2 f_uv;

void main(void){
  gl_Position = vec4(i_position.x * 2.0 - 1.0, i_position.y * -2.0 + 1.0, 0.0, 1.0);
  f_uv = i_texture;
}
