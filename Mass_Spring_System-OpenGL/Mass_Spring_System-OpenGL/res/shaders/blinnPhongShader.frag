#version 460

// Vertex pos in view space
in vec3 position;
// Vertex normal
in vec3 normal;
// Light direction in view space
in vec3 lightDir;

// Light colors
uniform vec3 lightAmbient;			// Ambient light intensity
uniform vec3 lightDiffuse;			// Diffuse and specular light intensity

// Material params
uniform vec3 matAmbient;			// Ambient reflectivity
uniform vec3 matDiffuse;			// Diffuse reflectivity
uniform vec3 matSpecular;			// Specular reflectivity
uniform float matShininess;			// Specular shininess factor

// Final color
out vec4 fragColor;

// Calculate light equation of the blinn-phong model
vec3 blinnPhong ( vec3 position, vec3 normal ) 
{
	// View vector 
	vec3 viewDir = normalize(-position.xyz);
	
	//Ambient component color
    vec3 ambientComponent = lightAmbient * matAmbient;

    //Bi-side lighting
    float viewNormalDot = dot ( viewDir, normal);
	if ( viewNormalDot < 0 ) {
		normal = -normal;
	}

    //lambertian coefficient
	float lightNormalDot = max ( dot ( lightDir, normal ), 0.0 );
	
	// Specular component color
    vec3 specularComponent = vec3(0.0);

    if ( lightNormalDot > 0.0 ) {
		// Halfway vector 
		vec3 halfway = normalize( viewDir + lightDir );

		float specAngle=max( dot(halfway, normal), 0.0);
		float specularCoefficient = pow( specAngle, matShininess);
		
		// Light specular color is the same of the light diffuse color in this case
		specularComponent = matSpecular * lightDiffuse * specularCoefficient;
	}
    
	// Diffuse component color
	vec3 diffuseComponent = matDiffuse * lightDiffuse * lightNormalDot;

	// Output color
	vec3 color = ambientComponent + diffuseComponent + specularComponent;

	return color;
}

void main() 
{
	fragColor = vec4 ( blinnPhong (position, normalize(normal) ), 1);
}	