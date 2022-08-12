#pragma once
#include <map>
#include <vector>

#include "Vertex.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Material.h"
#include "glad/glad.h"

class Mesh {
private:
	std::vector<Vertex> m_Vertices;
	std::vector<GLuint> m_Indices;

	//TODO: move on Mass Spring class
	//Map<indice vertice, se è pinnato>
	//std::map<int, bool> pinnedVertices;

	Material m_Material;

public:
	VertexBuffer m_vbo;
	IndexBuffer m_indexBuffer;
	VertexArray m_vao;

	std::vector<Vertex>& GetVertices() { return m_Vertices; }
	std::vector<GLuint>& GetIndices() { return m_Indices; }

	VertexBuffer& GetVertexBuffer() { return m_vbo; }
	IndexBuffer& GetIndexBuffer() { return m_indexBuffer; }
	VertexArray& GetVertexArray() { return m_vao; }

	Material& GetMaterial() { return m_Material; }

	GLfloat GetSize() const { return m_Vertices.size(); }

	void SetBuffers(const VertexBufferLayout& layout)
	{
		GLsizei verticesSize = m_Vertices.size() * sizeof(Vertex);
		m_vbo.SetData (m_Vertices.data(), verticesSize);
		
		GLuint indicesSize = m_Indices.size() * sizeof(GLuint);
		m_indexBuffer.SetData (m_Indices.data(), indicesSize);

		m_vao.AddVertexBuffer(m_vbo, layout);
	}

	void UpdateBuffers()
	{
		GLsizei verticesSize = m_Vertices.size() * sizeof(Vertex);
		m_vbo.UpdateData (m_Vertices.data(), verticesSize);
	}
};