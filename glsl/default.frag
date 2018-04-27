#version 430

uniform sampler2D u_tex0;
uniform float u_time;

in vec2 v_texcoord;

out vec4 fragment_color;

void main () {
   fragment_color = texture(u_tex0, v_texcoord);
}
