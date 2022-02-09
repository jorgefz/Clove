
#type vertex

#version 330 core

// Attributes
layout(location = 0) in vec3 a_pos;

// Uniforms
uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

void main() {
	gl_Position = u_ViewProjection * u_Transform * vec4(a_pos, 1.0);
}



#type fragment

#version 330 core

layout(location = 0) out vec4 color;

uniform vec4 u_color;
 
void main() {
	color = u_color;
}

