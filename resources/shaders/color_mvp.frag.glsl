#version 330 core

layout(location = 0) out vec4 color;
in vec3 v_pos;
in vec4 v_color;

uniform vec4 u_color;

void main() {
	if (u_color.r == 0.0f && u_color.g == 0.0f && u_color.b == 0.0f ){
		color = v_color;
	}
	else color = u_color;
}