#version 460

in vec3 position;
in vec3 normal;

out vec4 fragColor;

void main() 
{
	fragColor = vec4 ( normal, 1);
}	