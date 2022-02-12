#type vertex

#version 330 core

// Attributes
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;
layout(location = 2) in vec4 aColor;
layout(location = 3) in float aTextureID;
layout(location = 4) in float aTilingFactor;

// Uniforms
uniform mat4 uViewProjection;

// outputs (aka varying)
out vec2 vTexCoord;
out vec4 vColor;
out float vTextureID;
out float vTilingFactor;

void main() {
	vTexCoord = aTexCoords;
	vColor = aColor;
	vTextureID = aTextureID;
	vTilingFactor = aTilingFactor;
	gl_Position = uViewProjection * vec4(aPos, 1.0);
}


#type fragment

#version 330 core

layout(location = 0) out vec4 color;
in vec2 vTexCoord;
in vec4 vColor;
in float vTextureID;
in float vTilingFactor;

uniform sampler2D uTextures[32];

void main() {
	color = texture(uTextures[int(vTextureID)], vTexCoord * vTilingFactor) * vColor;
}