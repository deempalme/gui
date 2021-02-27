#version 420 core

layout(location = 0) in vec4 i_xy_size;
layout(location = 1) in vec4 i_texture;
layout(location = 2) in float i_id;

out vec4 g_texture;
out int  g_id;

void main(void){
  gl_Position = i_xy_size;
  g_id = int(round(i_id));
  g_texture = i_texture;
}
