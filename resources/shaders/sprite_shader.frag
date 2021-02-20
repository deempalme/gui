#version 420 core

uniform float u_parent_id;
uniform sampler2D u_image;

in vec2 f_uv;
in float f_id;
in vec2 f_xy;

layout(location = 0) out vec4 f_color;
layout(location = 1) out vec4 f_background;

void main(void){
  f_color = texture(u_image, f_uv);
  f_background = vec4(u_parent_id, f_id, f_xy.x, f_xy.y);
}
