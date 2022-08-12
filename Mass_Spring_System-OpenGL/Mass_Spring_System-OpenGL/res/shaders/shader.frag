#version 460

in vec3 position;
in vec3 normal;

uniform vec3 lightPosition;			// Light position in camera coordinates
uniform vec3 lightAmbient;			// Ambient light intensity
uniform vec3 light;					// Diffuse and specular light intensity

uniform vec3 matAmbient;			// Ambient reflectivity
uniform vec3 matDiffuse;			// Diffuse reflectivity
uniform vec3 matSpecular;			// Specular reflectivity
uniform float matShininess;			// Specular shininess factor

out vec4 fragColor;

vec3 blinnPhong ( vec3 position, vec3 normal ) 
{
	vec3 diffuseReflectivity = vec3 ( 1.0, 1.0, 1.0);
	
	vec3 ambient = lightAmbient * matAmbient;

	vec3 lightSourceDir = normalize( lightPosition - position );
	float sourceDir_dot_normal = max ( dot ( lightSourceDir, normal ), 0.0 );

	vec3 diffuse = matDiffuse * sourceDir_dot_normal;

	vec3 specular = vec3(0.0);

	if ( sourceDir_dot_normal > 0.0 ) {
		vec3 cameraDir = normalize(-position.xyz);
		vec3 halfway = normalize( cameraDir + lightSourceDir );
		specular = matSpecular * pow( max( dot(halfway, normal), 0.0), matShininess);
	}

	return ambient + light * (diffuse + specular);
}

void main() 
{
	fragColor = vec4 ( blinnPhong (position, normalize(normal) ), 1);
}	