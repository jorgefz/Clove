#version 330 core

// Attributes
layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec4 a_color;

// Varying
out vec3 v_pos;
out vec4 v_color;

void main() {
	v_pos = a_pos;
	v_color = a_color;
	gl_Position = vec4(a_pos, 1.0);
}