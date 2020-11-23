#version 420 core

layout(location = 0) in vec2 i_position;
layout(location = 1) in vec4 i_texture;
layout(location = 2) in vec2 i_size;

out vec4 g_texture;
out vec2 g_size;


void main(void){
  gl_Position = vec4(i_position, 0.0, 1.0);
  g_texture = i_texture;
  g_size = i_size;
}
