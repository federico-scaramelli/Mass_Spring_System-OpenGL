#include "VertexArray.h"

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "../VertexStructures.h"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_bufferID);
	glBindVertexArray(m_bufferID);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_bufferID);
}

void VertexArray::AddBuffer(VertexBuffer& vertexBuffer, VertexBufferLayout& layout)
{
	Bind();
	
	vertexBuffer.BindToVao(m_bufferID, 0, layout.GetStride());
 //
	// glEnableVertexAttribArray(0);
	// glEnableVertexAttribArray(1);
	//
	// glVertexArrayAttribBinding(m_bufferID, 0, 0);
 //    glVertexArrayAttribBinding(m_bufferID, 1, 0);
 //
	// glVertexArrayAttribFormat(m_bufferID, 0, glm::vec3::length(), GL_FLOAT, GL_FALSE,
 //                              offsetof(QuadAttributesStruct, position));
 //    glVertexArrayAttribFormat(m_bufferID, 1, glm::vec3::length(), GL_FLOAT, GL_FALSE,
 //                              offsetof(QuadAttributesStruct, color)); 


	GLuint offset = 0;
	const auto& elements = layout.GetElements();
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		auto t=glm::vec3::length();
		glEnableVertexAttribArray(i);
		glVertexArrayAttribBinding(m_bufferID, i, 0);
		glVertexArrayAttribFormat(m_bufferID, i, element.count, GL_FLOAT, GL_FALSE, offset);

		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_bufferID);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}
