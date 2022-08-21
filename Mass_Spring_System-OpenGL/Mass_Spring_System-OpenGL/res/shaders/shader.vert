#version 460

layout(location = 0) in vec4 vertexPosition;
layout(location = 1) in vec4 vertexVelocity;
layout(location = 2) in vec4 vertexColor;
layout(location = 3) in vec4 vertexNormal;
layout(location = 4) in vec4 vertexOldPosition;
layout(location = 5) in vec4 pinned;

out vec3 position;
out vec3 normal;
out vec3 velocity;

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;

void main() 
{
	vec4 camSpaceCoords = modelViewMatrix * vec4(vertexPosition.xyz, 1.0);
	vec4 clipSpaceCoordinates = projectionMatrix * camSpaceCoords;

	normal = normalize (  normalMatrix * vertexNormal.xyz );
	position = camSpaceCoords.xyz;
	velocity = vertexVelocity.xyz;

	gl_Position = clipSpaceCoordinates;
}	
