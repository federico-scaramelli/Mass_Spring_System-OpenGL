#version 460

in vec3 position;
in vec3 normal;

uniform vec3 solidColor;

out vec4 fragColor;

void main() 
{
	fragColor = vec4 ( solidColor, 1);
}	