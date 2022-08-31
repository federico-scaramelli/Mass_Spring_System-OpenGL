#version 460

// Per vertex params
layout(location = 0) in vec4 vertexPosition;
layout(location = 1) in vec4 vertexVelocity;
layout(location = 2) in vec4 vertexColor;
layout(location = 3) in vec4 vertexNormal;
layout(location = 4) in vec4 vertexOldPosition;
layout(location = 5) in vec4 pinned;

// Output info for the fragment shader
out vec3 position;
out vec3 normal;
out vec3 velocity;
out vec3 lightDir;

// Light position 
uniform vec3 lightPosition;			

// Model-view transformation matrix
uniform mat4 modelViewMatrix;

// Perspective projection matrix
uniform mat4 projectionMatrix;

// Normal matrix
uniform mat3 normalMatrix;

void main() 
{
	// Vertex position in view space
	vec4 camSpaceCoords = modelViewMatrix * vec4(vertexPosition.xyz, 1.0);
	
	// Vertex position in clip space
	vec4 clipSpaceCoordinates = projectionMatrix * camSpaceCoords;

	// Calculate normal in world space
	normal = normalize (  normalMatrix * vertexNormal.xyz );

	// Set outputs variables
	position = camSpaceCoords.xyz;
	velocity = vertexVelocity.xyz;
	lightDir = normalize( lightPosition - camSpaceCoords.xyz);

	// Set vertex position
	gl_Position = clipSpaceCoordinates;
}	
