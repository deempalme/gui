#version 420 core

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

//uniform float u_whole;

layout(std140, binding = 1) uniform u_scene {
  vec2 u_window_size;
  vec2 u_sprite_size;
};

in vec4 g_texture[];
in float g_id[];

out vec2 f_uv;
out float f_id;
out vec2 f_xy;

void main(void){
  vec2 position = gl_in[0].gl_Position.xy;
  vec2 size = gl_in[0].gl_Position.zw;

  if(size.x > 1.0) size.x /= u_window_size.x;
  if(size.y > 1.0) size.y /= u_window_size.y;

  // Calculating horizontal position
  if(position.x < 0.0) position.x = (u_window_size.x + position.x) / u_window_size.x - size.x;
  else if(position.x > 1.0) position.x /= u_window_size.x;
  // Calculating vertical position
  if(position.y < 0.0) position.y = (u_window_size.y + position.y) / u_window_size.y - size.y;
  else if(position.y > 1.0) position.y /= u_window_size.y;

  // Calculating if texture's uv are using percentual values
  const vec4 uv = vec4(
    g_texture[0].x <= 1.0 ? g_texture[0].x : g_texture[0].x / u_sprite_size.x,
    g_texture[0].y <= 1.0 ? g_texture[0].y : g_texture[0].y / u_sprite_size.y,
    g_texture[0].z <= 1.0 ? g_texture[0].z : g_texture[0].z / u_sprite_size.x,
    g_texture[0].w <= 1.0 ? g_texture[0].w : g_texture[0].w / u_sprite_size.y
  );

  f_id = g_id[0];

  // bottom right corner
  f_uv = uv.zy;
  f_xy = vec2(1.0, 1.0);
  gl_Position = vec4((position.x + size.x) * 2.0 - 1.0, (position.y + size.y) * -2.0 + 1.0, 0.0, 1.0);
  EmitVertex();

  // top right corner
  f_uv = uv.zw;
  f_xy = vec2(1.0, 0.0);
  gl_Position = vec4((position.x + size.x) * 2.0 - 1.0, position.y * -2.0 + 1.0, 0.0, 1.0);
  EmitVertex();

  // bottom left corner
  f_uv = uv.xy;
  f_xy = vec2(0.0, 1.0);
  gl_Position = vec4(position.x * 2.0 - 1.0, (position.y + size.y) * -2.0 + 1.0, 0.0, 1.0);
  EmitVertex();

  // top left corner:
  f_uv = uv.xw;
  f_xy = vec2(0.0, 0.0);
  gl_Position = vec4(position.x * 2.0 - 1.0, position.y * -2.0 + 1.0, 0.0, 1.0);
  EmitVertex();

  EndPrimitive();
}
