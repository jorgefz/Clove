#version 330 core

layout(location = 0) out vec4 color;
in vec3 v_pos;

void main() {
	color = vec4(v_pos + 0.5, 1.0);
}