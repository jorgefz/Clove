#version 330 core

// Fragment shader: applied on each pixel individually

layout(location = 0) out vec4 color;
in vec2 v_tex_coord; //from vertex shader, texture coordinate to apply
uniform vec4 u_color;
uniform sampler2D u_texture; //texture to sample

void main() {
	vec4 tex_color = texture(u_texture, v_tex_coord);
	//color = u_color;
	color = tex_color;
}