#version 460

in vec3 color;
in vec3 light;

out vec4 fragColor;

void main() 
{
	fragColor = vec4(light, 1.0);
}	