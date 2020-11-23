#version 420 core

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

in vec4 g_texture[];
in vec2 g_size[];

uniform float u_size;

out vec2  f_texture;
out int   f_id;
out float f_horizontal;

layout (std140, binding = 1) uniform u_window {
  vec2  u_window_size;
  float u_font_size;
};

void main(void){
  f_id = gl_PrimitiveIDIn;
  const vec2 position = gl_in[0].gl_Position.xy;

  // bottom right corner
  f_texture = g_texture[0].zw;
  f_horizontal = 1.0;
  gl_Position = vec4(position, 0.0, 1.0);
  EmitVertex();

  // top right corner
  f_texture = g_texture[0].zy;
  gl_Position = u_projection * vec4(quat_rot(u_rotation,
                                             vec3(position.x + g_size[0].x,
                                                  position.y,
                                                  position.z)) + u_translation, 1.0);
  EmitVertex();

  // bottom left corner
  f_texture = g_texture[0].xw;
  f_horizontal = 0.0;
  gl_Position = u_projection * vec4(quat_rot(u_rotation,
                                             vec3(position.x,
                                                  position.y - g_size[0].y,
                                                  position.z)) + u_translation, 1.0);
  EmitVertex();

  // top left corner:
  f_texture = g_texture[0].xy;
  gl_Position = u_projection * vec4(quat_rot(u_rotation,
                                             vec3(position.x,
                                                  position.y,
                                                  position.z)) + u_translation, 1.0);
  EmitVertex();

  EndPrimitive();
}
