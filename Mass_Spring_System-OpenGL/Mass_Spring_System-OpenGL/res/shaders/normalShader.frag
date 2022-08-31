#version 460

// Vertex pos in view space
in vec3 position;
// Vertex normal
in vec3 normal;

// Final color
out vec4 fragColor;

void main() 
{
	// View vector 
	vec3 viewDir = normalize(-position.xyz);
	
	vec3 bisideNormal=normal;

	//Bi-side lighting
    float viewNormalDot = dot ( viewDir, normal);
	if ( viewNormalDot < 0 ) {
		bisideNormal = -normal;
	}

	fragColor = vec4 ( bisideNormal, 1);
}	