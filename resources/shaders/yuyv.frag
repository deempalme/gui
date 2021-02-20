#version 420 core

//uniform float u_parent_id;
uniform sampler2D yuyv_image;
uniform vec2 u_yuyv_size;

in vec2 f_uv;
in float f_id;
in vec2 f_xy;

layout(location = 0) out vec4 f_color;
layout(location = 1) out vec4 f_background;

void main() {
  const vec4 pixel = texture(yuyv_image, f_uv);
  const float cb = pixel.g * 255;
  const float cr = pixel.a * 255;

  const float n = float(int(round(u_yuyv_size.x * f_uv.x)) % 2);
  const float y = mix(pixel.r, pixel.b, n) * 255;

  // Method #1
  f_color.r = (y + (1.4065 * (cr - 128))) / 255;
  f_color.g = (y - (0.3455 * (cb - 128)) - (0.7169 * (cr - 128))) / 255;
  f_color.b = (y + (1.7790 * (cb - 128))) / 255;

  // Method #2
  f_color.r = (1.164 * (y - 16.0) + 1.596 * (cr - 128.0)) / 255;
  f_color.g = (1.164 * (y - 16.0) - 0.813 * (cr - 128.0) - 0.391 * (cb - 128.0)) / 255;
  f_color.b = (1.164 * (y - 16.0) + 2.018 * (cb - 128.0)) / 255;

  f_color.a = 1.0;
  f_background = vec4(/*u_parent_id*/0.0, f_id, f_xy.x, f_xy.y);
}
