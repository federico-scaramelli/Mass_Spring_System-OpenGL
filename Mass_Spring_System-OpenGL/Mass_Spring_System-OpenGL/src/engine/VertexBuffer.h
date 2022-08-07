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

	GLuint GetSize()
	{
		switch (type)
		{
			case GL_FLOAT: return 4;
			case GL_UNSIGNED_INT: return 4;
			case GL_UNSIGNED_BYTE: return 1;
		}

		ASSERT(false);
		return 0;
	}

	GLuint GetStride()
	{
		return GetSize() * count;
	}
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
	void SetFormat(VertexBufferFormat& format);
	void SetFormat(GLuint type, GLuint count, GLboolean normalized);

	void Bind(GLuint bindingIdx, GLuint offset, GLuint stride) const; 
	void Unbind() const;
};

