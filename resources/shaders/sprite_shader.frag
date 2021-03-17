#version 420 core

uniform int u_parent_id;
uniform sampler2D u_image;

in vec2 f_uv;
flat in int f_id;
in vec2 f_xy;

layout(location = 0) out vec4 f_color;
layout(location = 1) out ivec4 f_background;

void main(void){
  // TODO: change channels order
  f_color = texture(u_image, f_uv).bgra;
  f_background = ivec4(u_parent_id, f_id, int(f_xy.x * 100.0), int(f_xy.y * 100.0));
}
