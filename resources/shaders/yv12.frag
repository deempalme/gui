#version 420 core

//uniform int u_parent_id;
uniform sampler2D y_tex;
uniform sampler2D u_tex;
uniform sampler2D v_tex;

in vec2 f_uv;
flat in int f_id;
in vec2 f_xy;

layout(location = 0) out vec4 o_color;
layout(location = 1) out ivec4 o_background;

void main() {
  const float y = texture(y_tex, f_uv).r;
  const float u = texture(u_tex, f_uv).r;
  const float v = texture(v_tex, f_uv).r;

  o_color.r = y + 1.13983 * (v - 0.5);
  o_color.g = y - 0.39465 * (u - 0.5) - 0.58060 * (v - 0.5);
  o_color.b = y + 2.03211 * (u - 0.5);

  o_color.a = 1.0;
  o_background = ivec4(/*u_parent_id*/0, f_id, int(f_xy.x * 10000.0), int(f_xy.y * 10000.0));
}
