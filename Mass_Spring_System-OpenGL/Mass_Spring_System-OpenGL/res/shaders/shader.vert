#version 460

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexColor;

out vec3 color;

//uniform mat4 rotationMatrix;

void main() 
{
	color = vertexColor;

	vec4 finalPosition = /*rotationMatrix * */vec4(vertexPosition, 1.0);

	gl_Position = finalPosition;
}	