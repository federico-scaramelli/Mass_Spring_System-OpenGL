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

	//State of compute buffer/secondary buffer: if 0 normal, if 1 inverted
	GLuint readBuf=0;
	GLuint m_SecondaryVertexBuffer;

	std::vector<Vertex>& GetVertices() { return m_Vertices; }
	std::vector<GLuint>& GetIndices() { return m_Indices; }

	VertexBuffer& GetVertexBuffer() { return m_vbo; }
	IndexBuffer& GetIndexBuffer() { return m_indexBuffer; }
	VertexArray& GetVertexArray() { return m_vao; }

	Material& GetMaterial() { return m_Material; }

	GLfloat GetSize() const { return m_Vertices.size(); }

	void SetBuffers(const VertexBufferLayout& layout)
	{
		//Vertex buffer
		GLsizei verticesSize = m_Vertices.size() * sizeof(Vertex);
		m_vbo.SetData (m_Vertices.data(), verticesSize);
		m_vao.AddVertexBuffer(m_vbo, layout);

		//Index buffer
		GLuint indicesSize = m_Indices.size() * sizeof(GLuint);
		m_indexBuffer.SetData (m_Indices.data(), indicesSize);
	}

	void SetComputeBuffers()
	{
		//Compute Buffers

		//In
		glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 0, m_vbo.GetID());
		glBufferData (GL_SHADER_STORAGE_BUFFER, m_vbo.GetSize(), m_Vertices.data(), GL_DYNAMIC_DRAW);

		//Out
		glGenBuffers(1, &m_SecondaryVertexBuffer);
		glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 1, m_SecondaryVertexBuffer);
		glBufferData (GL_SHADER_STORAGE_BUFFER, m_vbo.GetSize(), m_Vertices.data(), GL_DYNAMIC_DRAW);
	}

	void SwapComputeBuffers()
	{
	    readBuf = 1 - readBuf;

		if(readBuf==1)
		{
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_SecondaryVertexBuffer);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_vbo.GetID());
		}else
		{
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_vbo.GetID());
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_SecondaryVertexBuffer);
		}
	}

	void UpdateBuffers()
	{
		GLsizei verticesSize = m_Vertices.size() * sizeof(Vertex);
		m_vbo.UpdateData (m_Vertices.data(), verticesSize);
	}
};