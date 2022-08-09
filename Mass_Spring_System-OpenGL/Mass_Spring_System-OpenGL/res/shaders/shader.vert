#version 460

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexColor;
layout(location = 2) in vec3 vertexNormal;
layout(location = 3) in vec2 vertexUV;
out vec3 color;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;

void main() 
{
	color = vertexColor;

	vec4 worldSpacePosition = modelMatrix * vec4(vertexPosition, 1.0);
	vec4 screenSpacePosition = projectionMatrix * viewMatrix * worldSpacePosition;
	gl_Position = screenSpacePosition;
}	