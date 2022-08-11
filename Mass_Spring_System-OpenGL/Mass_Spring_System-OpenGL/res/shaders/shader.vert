#version 460

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexColor;
layout(location = 2) in vec3 vertexNormal;
layout(location = 3) in vec2 vertexUV;

out vec3 color;
out vec3 light;

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;

uniform vec3 lightPosition;
uniform vec3 lightAmbient;
uniform vec3 lightDiffuse;
uniform vec3 lightSpecular;

uniform vec3 matAmbient;
uniform vec3 matDiffuse;
uniform vec3 matSpecular;
uniform float matShininess;

void main() 
{
	vec3 diffuseReflectivity = vec3 ( 1.0, 1.0, 1.0);
	vec4 camCoords = modelViewMatrix * vec4(vertexPosition, 1.0);
	vec4 clipSpaceCoordinates = projectionMatrix * camCoords;
	
	vec3 normal = normalize( normalMatrix * vertexNormal);
	vec3 ambient = lightAmbient * matAmbient;
	vec3 lightSourceDir = normalize( vec3( vec4 (lightPosition, 1.0) - camCoords));
	float sourceDir_dot_normal = max ( dot ( lightSourceDir, normal ), 0.0 );
	vec3 diffuse = lightDiffuse * matDiffuse * sourceDir_dot_normal;
	vec3 specular = vec3(0.0);

	if ( sourceDir_dot_normal > 0.0 ) {
		vec3 v = normalize(-camCoords.xyz);
		vec3 r = reflect(-lightSourceDir, normal);
		specular = lightSpecular * matSpecular * pow( max( dot(r,v), 0.0), matShininess);
	}

	light = ambient + diffuse + specular;
	color = vertexColor;

	gl_Position = clipSpaceCoordinates;
}	