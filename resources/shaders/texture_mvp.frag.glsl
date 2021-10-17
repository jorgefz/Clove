#version 330 core

layout(location = 0) out vec4 color;
in vec3 v_pos;
in vec2 v_tex_coord;

uniform sampler2D u_texture;
 
void main() {
	color = texture(u_texture, v_tex_coord);
}