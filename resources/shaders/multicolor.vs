#version 330 core

// Vertex shader: fist step in shading. 

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 tex_coord;

out vec2 v_tex_coord; //sending to fragment shader
uniform mat4 u_mvp; //model view projection matrix

void main() {
	gl_Position = u_mvp * position;
	v_tex_coord = tex_coord;
};