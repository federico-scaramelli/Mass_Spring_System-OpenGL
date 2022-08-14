#pragma once

#include <random>

#include "Mesh.h"
#include "Renderer.h"
#include "Transform.h"

static std::random_device rd;
static std::mt19937 cpuGenerator(rd());
static std::uniform_real_distribution<float> unif{0, 1.0};

//Will be used as uniform
struct PhysicsParameters {
	GLfloat deltaTime = 0.016f;
	GLfloat stiffness = 1.f;
	GLfloat restLengthHorizontal = 1.f;
	GLfloat restLengthVertical = 1.f;
	GLfloat restLengthDiagonal = 1.f;
	GLfloat particleMass = 1;
	GLfloat damping=0.98f;
	glm::vec4 gravityAccel {0.f,-9.81f,0.f, 0.f};
};

class GameObject {
protected:
	Transform m_Transform;
	Mesh m_Mesh;

public:
	const char* name;

	void SetDefaultMesh ()
	{
		// SetCubeMesh();
		SetSphereMesh();
	}

	GameObject(const char* name) : name(name)
	{
		m_Transform = {name};
		SetDefaultMesh();
	}

	Transform& GetTransform()
	{
		return m_Transform;
	}

	Mesh& GetMesh()
	{
		return m_Mesh;
	}

	virtual void GenerateUI(Renderer& renderer)
	{
		//TODO: generate text with GO's name to define its section on the GUI

		m_Transform.GenerateUI (renderer);

		m_Mesh.GetMaterial().GenerateUI (renderer, name);
	}

	virtual void UpdateWithUI()
	{
		m_Transform.UpdateWithUI();

		m_Mesh.GetMaterial().UpdateWithUI();
	}

	glm::vec3 GetRandomColor()
	{
		return {
			unif(cpuGenerator),
			unif(cpuGenerator),
			unif(cpuGenerator)
		};
	}

	void SetColor(glm::vec3 color)
	{
		/*this->m_Mesh.GetMaterial().m_DiffuseColor = color;
		this->m_Mesh.GetMaterial().m_AmbientColor = color;

		for (Vertex& v : m_Mesh.GetVertices())
		{
			v.color = color;
		}

		m_Mesh.UpdateBuffers();*/
	}

	void SetCubeMesh()
	{
		auto& vertices = m_Mesh.GetVertices();
		vertices.clear();

		std::vector<Vertex> cubeVertices = {
			{{-0.5f, -0.5f, 0.5f, 0.f}}, 
			{{0.5f, -0.5f, 0.5f, 0.f}},  
			{{0.5f, 0.5f, 0.5f, 0.f}},  
			{{-0.5f, 0.5f, 0.5f, 0.f}}, 
			{{-0.5f, -0.5f, -0.5f, 0.f}}, 
			{{0.5f, -0.5f, -0.5f, 0.f}},  
			{{0.5f, 0.5f, -0.5f, 0.f}},
			{{-0.5f, 0.5f, -0.5f, 0.f}} 
		};

		vertices.assign(cubeVertices.begin(), cubeVertices.end());

		auto& indices = m_Mesh.GetIndices();
		indices = {
			//front
			0, 1, 2,
			2, 3, 0,

			//right
			1, 5, 6,
			6, 2, 1,

			//left
			0, 3, 7,
			7, 4, 0,

			//up
			3, 2, 6,
			6, 7, 3,

			//down
			0, 4, 5,
			5, 1, 0,

			//back 
			5, 4, 7,
			7, 6, 5
		};
	}

	void SetSphereMesh()
	{
		//Vertici
		auto& vertices = m_Mesh.GetVertices();
		vertices.clear();

		float PI=3.14f;
		float radius=0.5f;
		int stackCount=8;
		int sectorCount=stackCount*3;
		
		float x, y, z, xy;                              // vertex position
		float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal

		float sectorStep = 2 * PI / sectorCount;
		float stackStep = PI / stackCount;
		float sectorAngle, stackAngle;

		for(int i = 0; i <= stackCount; ++i)
		{
		    stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
		    xy = radius * cosf(stackAngle);          // r * cos(u)
		    z = radius * sinf(stackAngle);           // r * sin(u)
			
		    for(int j = 0; j <= sectorCount; ++j)
		    {
				Vertex vertex{};

		        sectorAngle = j * sectorStep;           // starting from 0 to 2pi
				
		        x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
		        y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)

				vertex.position={x,y,z, 0};
				
		        nx = x * lengthInv;
		        ny = y * lengthInv;
		        nz = z * lengthInv;

				vertex.normal={nx,ny,nz, 0};

				vertices.push_back(vertex);
		    }
		}

		//Indici
		auto& indices = m_Mesh.GetIndices();
		indices.clear();
		
		int k1, k2;
		for(int i = 0; i < stackCount; ++i)
		{
		    k1 = i * (sectorCount + 1);     // beginning of current stack
		    k2 = k1 + sectorCount + 1;      // beginning of next stack

		    for(int j = 0; j < sectorCount; ++j, ++k1, ++k2)
		    {
		        // k1 => k2 => k1+1
		        if(i != 0)
		        {
		            indices.push_back(k1);
		            indices.push_back(k2);
		            indices.push_back(k1 + 1);
		        }

		        // k1+1 => k2 => k2+1
		        if(i != (stackCount-1))
		        {
		            indices.push_back(k1 + 1);
		            indices.push_back(k2);
		            indices.push_back(k2 + 1);
		        }
		    }
		}
	}

	ShaderProgram& GetShader()
	{
		return m_Mesh.GetMaterial().GetShader();
	}

	ShaderProgramCompute& GetComputeShader()
	{
		return m_Mesh.GetMaterial().GetComputeShader();
	}

	Material& GetMaterial ()
	{
		return m_Mesh.GetMaterial();
	}
};
