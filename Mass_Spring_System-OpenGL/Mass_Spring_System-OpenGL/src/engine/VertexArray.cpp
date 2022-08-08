#include "VertexArray.h"
#include "Renderer.h"


VertexArray::VertexArray()
{
	//Create and setup the vertex array object
	GLCall(glGenVertexArrays(1, &m_bufferID))
	GLCall(glBindVertexArray(m_bufferID))
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_bufferID))
}

void VertexArray::AddBuffer(VertexBuffer& vb)
{
	Bind();

	GLCall(glEnableVertexAttribArray(vboCount));
	auto& format = vb.GetFormat();
	vb.Bind(vboCount);

	GLCall(glVertexAttribFormat(vboCount, format.count, format.type, format.normalized, 0));
	GLCall(glVertexAttribBinding(vboCount, vboCount));

	vboCount++;

	/*for (unsigned int i = 0; i < elements.size(); i++) 
	{
		const auto& element = elements[i];

		GLCall(glEnableVertexAttribArray(i));
		vb.Bind(i, offset, layout.GetStride());

		GLCall(glVertexAttribFormat(i, element.count, element.type, element.normalized, layout.GetStride()));
		GLCall(glVertexAttribBinding(i, i));

		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}*/
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_bufferID))
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0))
}
