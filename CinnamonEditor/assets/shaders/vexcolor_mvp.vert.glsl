#version 330 core

// Attributes
layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec4 a_color;

// Uniforms
uniform mat4 u_view_proj;
uniform mat4 u_transform;

uniform vec4 u_color1;
uniform vec4 u_color2;
uniform vec4 u_color3;
uniform vec4 u_color4;

// outputs
out vec3 v_pos;
out vec4 v_color;

void main() {
	if      (gl_VertexID == 0) v_color = u_color1;
	else if (gl_VertexID == 1) v_color = u_color2;
	else if (gl_VertexID == 2) v_color = u_color3;
	else if (gl_VertexID == 3) v_color = u_color4;

	v_pos = a_pos;
	//v_color = a_color;
	gl_Position = u_view_proj * u_transform * vec4(a_pos, 1.0);
}