#version 460

layout(location = 0) in vec4 vertexPosition;
layout(location = 1) in vec4 vertexVelocity;
layout(location = 2) in vec4 vertexColor;
layout(location = 3) in vec4 vertexNormal;
layout(location = 4) in vec2 vertexUV;
layout(location = 5) in float pinned;
layout(location = 6) in float dummy;

out vec3 position;
out vec3 normal;

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;

void main() 
{
	normal = normalize (  normalMatrix * vertexNormal.xyz );
	position = ( modelViewMatrix * vec4 ( vertexPosition.xyz, 1.0 ) ).xyz;

	vec4 camCoords = modelViewMatrix * vec4(vertexPosition.xyz, 1.0);
	vec4 clipSpaceCoordinates = projectionMatrix * camCoords;

	gl_Position = clipSpaceCoordinates;
}	