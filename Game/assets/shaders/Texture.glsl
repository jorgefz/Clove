#type vertex

#version 330 core

// Attributes
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;

// Uniforms
uniform mat4 uViewProjection;
uniform mat4 uTransform;

// outputs (aka varying)
out vec3 vPos;
out vec2 vTexCoord;

void main() {
	vPos = aPos;
	vTexCoord = aTexCoord;
	gl_Position = uViewProjection * uTransform * vec4(aPos, 1.0);
}


#type fragment

#version 330 core

layout(location = 0) out vec4 color;
in vec3 v_pos;
in vec2 vTexCoord;

uniform sampler2D uTexture;
uniform vec4 uColor = vec4(1.0, 1.0, 1.0, 1.0);

uniform float uTextureScale = 1.0;

void main() {
	color = texture(uTexture, vTexCoord * uTextureScale) * uColor;
}