#version 330 core
layout(location = 0) out vec4 color;
in vec2 v_texCoord;
uniform sampler2D u_Texture;
uniform vec4 u_Color;
void main() {
	vec4 texColor = texture(u_Texture, v_texCoord);
	if( texColor.a < 0.1 )
		discard;
	color = texColor;
};
