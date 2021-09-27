#version 330 core

// Attributes
layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec4 a_color;

// Uniforms
uniform mat4 u_view_proj;
uniform mat4 u_transform;

// Varying
out vec3 v_pos;
out vec4 v_color;

void main() {
	v_pos = a_pos;
	v_color = a_color;
	gl_Position = u_view_proj * u_transform * vec4(a_pos, 1.0);
}