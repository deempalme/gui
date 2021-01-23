#version 420 core

// GUI's id for this element
uniform int u_id;

uniform sampler2D u_image;
uniform vec4 u_border_color;
uniform vec4 u_background_color;

in vec2 f_texture;

layout(location = 0) out vec4 f_color;
layout(location = 1) out vec4 f_background;

void main(void){
  vec2 monochrome = texture(u_image, f_texture).ra;
  f_color = mix(u_background_color, u_border_color, monochrome.x) * monochrome.y;

  const float primary = float(u_id) / 256.0;
  const float secundary = (primary - floor(primary)) * 256.0;
  f_background = vec4(round(primary) / 256.0, secundary / 256.0, 0.0, 0.0);
}
