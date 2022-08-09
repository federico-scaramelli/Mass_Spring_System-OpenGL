#include "VertexArray.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Vertex.h"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_vaoID);
	glBindVertexArray(m_vaoID);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_vaoID);
}

void VertexArray::AddVertexBuffer(const VertexBuffer& vertexBuffer, const VertexBufferLayout& layout)
{
	/* The bindingIdx to create a correspondence between
	 * the VAO binding point and the attribute index.
	 * We can choose the bindingPoint freely.
	 * It must be only < GL_MAX_VERTEX_ATTRIB_BINDINGS.
	 * It's suggested to set it equal to the attribute index. */
	// TODO: check if it's needed to handle it != 0
	GLuint bindingPoint = 0;

	/* Specifies the ( binding-point <-> buffer ) correspondance.
	 * NB: The binding point is part of the VAO specififed in the
	 * first parameter and specifies a binding point to a VBO. */
	vertexBuffer.BindToVao(m_vaoID, bindingPoint, layout.GetStride());
 
	/* The distance in byte between different attributes of the same vertex*/
	GLuint offset = 0;
	const auto& elements = layout.GetElements();

	for (GLuint attribIdx = 0; attribIdx < elements.size(); attribIdx++)
	{
		const auto& element = elements[attribIdx];

		/* We could use glEnableVertexArrayAttrib(m_vaoID, i);
		 * exploiting the Direct State Access features of OpenGL 4.
		 * In that case there is no need to bind the vao,
		 * we access it directly specifying its ID. */
		//glEnableVertexAttribArray(attribIdx);
		glEnableVertexArrayAttrib(m_vaoID, attribIdx);
		/* Defines the format of an attribute in attribIdx on the VAO. */
		glVertexArrayAttribFormat(m_vaoID, attribIdx, element.count, GL_FLOAT, GL_FALSE, offset);
		/* Creates a correspondence between the attribute index and the
		 * binding point used in glVertexArrayVertexBuffer.
		 * The third parameter is the bindingIdx used in glVertexArrayVertexBuffer. */
		glVertexArrayAttribBinding(m_vaoID, attribIdx, bindingPoint);

		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_vaoID);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}
