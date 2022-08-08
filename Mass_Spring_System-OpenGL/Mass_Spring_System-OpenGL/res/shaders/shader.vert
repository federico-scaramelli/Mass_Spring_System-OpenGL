#version 460

in vec3 vertexColor;
in vec3 vertexPosition;

out vec3 color;

uniform mat4 rotationMatrix;

void main() 
{
	color = vertexColor;

	vec4 finalPosition = rotationMatrix * vec4(vertexPosition, 1.0);

	gl_Position = finalPosition;
}	