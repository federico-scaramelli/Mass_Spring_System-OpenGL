#version 460

in vec3 position;
in vec3 normal;

out vec4 fragColor;

void main() 
{
	vec3 viewDir = normalize(-position.xyz);
	vec3 bisideNormal=normal;

	//Bi-side lighting
    float viewNormalDot = dot ( viewDir, normal);
	if ( viewNormalDot < 0 ) {
		bisideNormal = -normal;
	}

	fragColor = vec4 ( bisideNormal, 1);
}	