#pragma once
#include "Renderer.h"
#include "glad/glad.h"

struct VertexBufferFormat
{
	//Type ov values conteined
	GLuint type;
	//Count of values contained, for a vec3 is 3
	GLuint count;
	GLboolean normalized;

	GLuint stride;
};

class VertexBuffer
{
private:
	/* Renderer ID is the ID related to the renderer, in our case OpenGL */
	GLuint m_rendererID;
	VertexBufferFormat m_format;

public:
	//Need size cause void* locks the sizeof call
	VertexBuffer(const void* data, const GLuint size);
	~VertexBuffer();

	VertexBufferFormat& GetFormat();

	template <class T>
	void SetFormat(GLuint count);

	void SetFormat(VertexBufferFormat& format);

	void Bind(GLuint bindingIdx, GLuint offset = 0) const; 
	void Unbind() const;
};

