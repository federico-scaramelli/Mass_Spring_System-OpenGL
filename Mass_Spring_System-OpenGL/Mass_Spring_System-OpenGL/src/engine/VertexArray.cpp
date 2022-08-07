#include "VertexArray.h"
#include "Renderer.h"


VertexArray::VertexArray()
{
	//Create and setup the vertex array object
	GLCall(glGenVertexArrays(1, &m_RendererID))
	GLCall(glBindVertexArray(m_RendererID))
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID))
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();

	const auto& elements = layout.GetElements();
	GLuint offset = 0;

	GLuint i = layout.GetElements().size() - 1;
	const auto& element = elements[i];
	GLCall(glEnableVertexAttribArray(i));
	vb.Bind(i, offset, VertexBufferElement::GetSizeOfType(GL_FLOAT) * element.count);

	GLCall(glVertexAttribFormat(i, element.count, element.type, element.normalized, 
	  VertexBufferElement::GetSizeOfType(GL_FLOAT) * element.count));
	GLCall(glVertexAttribBinding(i, i));

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
	GLCall(glBindVertexArray(m_RendererID))
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0))
}
