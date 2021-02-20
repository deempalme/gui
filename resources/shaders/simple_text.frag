#version 420 core

uniform int u_parent_id;

uniform sampler2D u_atlas;
uniform vec4 u_color;

in vec2 f_texture;
in int f_id;
in float f_horizontal;

layout(location = 0) out vec4 f_color;
layout(location = 1) out vec3 f_background;

const float width = 0.45;
const float edge = 0.1;

void main(void){
  float distance = 1.0 - texture(u_atlas, f_texture).a;
  float alpha = 1.0 - smoothstep(width, width + edge, distance);

  f_color = vec4(u_color.rgb, u_color.a * alpha);
  f_background = vec3((float)u_parent_id / 255.0, (float)f_id / 255.0, f_horizontal);
}
