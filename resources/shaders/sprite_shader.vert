#version 420 core

layout(location = 0) in vec4 i_xy_size;
layout(location = 1) in vec4 i_texture;
layout(location = 2) in vec4 i_color;
layout(location = 3) in vec2 i_offset;
layout(location = 4) in float i_id;

out vec4 g_texture;
out vec4 g_color;
out vec2 g_offset;
out int  g_id;

void main(void){
  gl_Position = i_xy_size;
  g_id = int(round(i_id));
  g_texture = i_texture;
  g_color = i_color;
  g_offset = i_offset;
}
