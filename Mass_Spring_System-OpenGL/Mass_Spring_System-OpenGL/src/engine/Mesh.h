#pragma once
#include <map>
#include <vector>

#include "Vertex.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "glad/glad.h"

class Mesh {
private:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

	//Map<indice vertice, se è pinnato>
	std::map<int, bool> pinnedVertices;

public:
	VertexBuffer vertexBuffer;
	IndexBuffer indexBuffer;
	VertexArray vertexArrayObject;

	std::vector<Vertex>& GetVertices() { return vertices; }

	std::vector<GLuint>& GetIndices() { return indices; }

	GLfloat GetSize() const { return vertices.size(); }

	void SetBuffers(VertexBufferLayout& layout)
	{
		GLsizei verticesSize = vertices.size() * sizeof(Vertex);
		vertexBuffer.SetData (vertices.data(), verticesSize);
		
		GLuint indicesSize = indices.size() * sizeof(GLuint);
		indexBuffer.SetData (indices.data(), indicesSize);

		vertexArrayObject.AddVertexBuffer(vertexBuffer, layout);
	}

	void UpdateBuffers()
	{
		GLsizei verticesSize = vertices.size() * sizeof(Vertex);
		vertexBuffer.UpdateData (vertices.data(), verticesSize);
	}
};