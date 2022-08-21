#version 460

in vec3 position;
in vec3 normal;
in vec3 velocity;

out vec4 fragColor;

void main() 
{
	float absVelocity = length( vec3(abs(velocity.x), abs(velocity.y), abs(velocity.z)));
	float maxVelocity=250;

	//vec3 baseColor = normalize(absVelocity);
	vec3 baseColor = vec3(0, 0, min( absVelocity/maxVelocity, 1));

	fragColor = vec4 ( baseColor, 1);
}	