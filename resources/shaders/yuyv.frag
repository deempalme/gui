#version 420 core

//uniform int u_parent_id;

uniform sampler2D yuyv_image;

uniform bool u_yuyv;
uniform float u_yuyv_size;

in vec2 f_uv;
flat in int f_id;
in vec2 f_xy;

layout(location = 0) out vec4 o_color;
layout(location = 1) out ivec4 o_background;

// Decoding yuyv data
void decode_yuyv(){
}

void main() {
  const vec4 pixel = texture(yuyv_image, f_uv);
  const float cb = pixel.g * 255;
  const float cr = pixel.a * 255;

  const float n = float(int(round(u_yuyv_size * f_uv.x)) % 2);
  const float y = mix(pixel.r, pixel.b, n) * 255;

  // Method #1
//  o_color.r = (y + (1.4065 * (cr - 128))) / 255;
//  o_color.g = (y - (0.3455 * (cb - 128)) - (0.7169 * (cr - 128))) / 255;
//  o_color.b = (y + (1.7790 * (cb - 128))) / 255;

  // Method #2
  o_color.r = (1.164 * (y - 16.0) + 1.596 * (cr - 128.0)) / 255;
  o_color.g = (1.164 * (y - 16.0) - 0.813 * (cr - 128.0) - 0.391 * (cb - 128.0)) / 255;
  o_color.b = (1.164 * (y - 16.0) + 2.018 * (cb - 128.0)) / 255;

  o_color.a = 1.0;
  o_background = ivec4(/*u_parent_id*/0, f_id, int(f_xy.x * 10000.0), int(f_xy.y * 10000.0));
}
