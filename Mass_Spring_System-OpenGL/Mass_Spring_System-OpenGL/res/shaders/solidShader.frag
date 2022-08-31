#version 460

// Vertex pos in view space
in vec3 position;
// Vertex normal
in vec3 normal;

// Color to assign
uniform vec3 solidColor;

out vec4 fragColor;

void main() 
{
	fragColor = vec4 ( solidColor, 1);
}	