#version 330 core

// Attributes
layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec2 a_tex_coord;

// Uniforms
uniform mat4 u_view_proj;
uniform mat4 u_transform;

// outputs (aka varying)
out vec3 v_pos;
out vec2 v_tex_coord;

void main() {
	v_pos = a_pos;
	v_tex_coord = a_tex_coord;
	gl_Position = u_view_proj * u_transform * vec4(a_pos, 1.0);
}