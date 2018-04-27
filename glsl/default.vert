#version 430

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec2 v_uv;

//uniform mat4 projection_mat, view_mat, model_mat;

out vec2 v_texcoord;

void main () {
	v_texcoord = v_uv;
	//vec3 position_eye = (view_mat * vec4 (v_position, 1.0)).xyz;
	gl_Position = vec4 (v_position, 1.0);
}
