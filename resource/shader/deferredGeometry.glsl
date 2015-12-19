#version 330
#extension GL_ARB_shading_language_420pack : enable

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Normal;
layout(location = 2) in vec4 in_Color;

struct VertexOut {
	vec3 position;
	vec3 normal;
	float depth;
	vec2 texCoords;
};