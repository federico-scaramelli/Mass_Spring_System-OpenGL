#version 460

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexColor;
layout(location = 2) in vec3 vertexNormal;
layout(location = 3) in vec2 vertexUV;

out vec3 position;
out vec3 normal;

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;

void main() 
{
	normal = normalize (  normalMatrix * vertexNormal );
	position = ( modelViewMatrix * vec4 ( vertexPosition, 1.0 ) ).xyz;

	vec4 camCoords = modelViewMatrix * vec4(vertexPosition, 1.0);
	vec4 clipSpaceCoordinates = projectionMatrix * camCoords;

	gl_Position = clipSpaceCoordinates;
}	