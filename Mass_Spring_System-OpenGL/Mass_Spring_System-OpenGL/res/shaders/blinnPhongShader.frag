#version 460

//posizione vertice in camera coords
in vec3 position;
in vec3 normal;

uniform vec3 lightPosition;			// Light position in camera coordinates
uniform vec3 lightAmbient;			// Ambient light intensity
uniform vec3 lightDiffuse;			// Diffuse and specular light intensity

uniform vec3 matAmbient;			// Ambient reflectivity
uniform vec3 matDiffuse;			// Diffuse reflectivity
uniform vec3 matSpecular;			// Specular reflectivity
uniform float matShininess;			// Specular shininess factor

out vec4 fragColor;

vec3 blinnPhong ( vec3 position, vec3 normal ) 
{
	//View vec
	vec3 viewDir = normalize(-position.xyz);
	//Light vec
	vec3 lightDir = normalize( lightPosition - position );
    
    vec3 ambientComponent = lightAmbient * matAmbient;
	vec3 specularComponent = vec3(0.0);

    //Bi-side lighting
    float viewNormalDot = dot ( viewDir, normal);
	if ( viewNormalDot < 0 ) {
		normal = -normal;
	}

    //lambertian coefficient
	float lightNormalDot = max ( dot ( lightDir, normal ), 0.0 );
	
    
    // if the lambert coefficient is positive, then I can calculate the specular component
    if ( lightNormalDot > 0.0 ) {
		vec3 halfway = normalize( viewDir + lightDir );

		float specAngle=max( dot(halfway, normal), 0.0);
		float specularCoefficient = pow( specAngle, matShininess);
		
		//LightSpecular===LightDiffuse in this case
		specularComponent = matSpecular * lightDiffuse * specularCoefficient;
	}
    
	vec3 diffuseComponent = matDiffuse * lightDiffuse * lightNormalDot;

	vec3 color = ambientComponent + diffuseComponent + specularComponent;

	return color;
}

void main() 
{
	fragColor = vec4 ( blinnPhong (position, normalize(normal) ), 1);
}	