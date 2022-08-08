#include "VertexArray.h"
#include "Renderer.h"


VertexArray::VertexArray()
{
	//Create and setup the vertex array object
	glGenVertexArrays(1, &m_bufferID);
	glBindVertexArray(m_bufferID);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_bufferID);
}

void VertexArray::AddBuffer(VertexBuffer& vb)
{
	Bind();
	
	glEnableVertexAttribArray(vboCount);
	auto& format = vb.GetFormat();
	vb.Bind(vboCount);

	glVertexAttribFormat(vboCount, format.count, format.type, format.normalized, 0);
	glVertexAttribBinding(vboCount, vboCount);

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
	glBindVertexArray(m_bufferID);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}
