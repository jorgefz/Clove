#version 330 core

layout(location = 0) out vec4 color;
in vec3 v_pos;
in vec4 v_color;

uniform vec4 u_color;
uniform int  u_use_vexcolor = 0;

void main() {
	
	if ( u_use_vexcolor == 1 ){
		color = v_color;
	}
	else color = u_color;
}