#version 420 core

uniform int u_parent_id;
uniform sampler2D u_image;

in vec2 f_uv;
in vec4 f_color;
in vec2 f_xy;
flat in int f_id;

layout(location = 0) out vec4 o_color;
layout(location = 1) out ivec4 o_background;

void main(void){
  // TODO: change channels order
  o_color = texture(u_image, f_uv) * f_color;
  o_background = ivec4(u_parent_id, f_id, int(f_xy.x * 10000.0), int(f_xy.y * 10000.0));
}
