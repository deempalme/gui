#version 420 core

uniform sampler2D u_image;

in vec2 f_uv;

layout(location = 0) out vec4 f_color;

void main(void){
  f_color = texture(u_image, f_uv);
}
