/*
 * ELTE IK Számítógépes grafika
 * World of Warships
 * Sándor Balázs
 * AZA6NL
 */

#include "WorldOfWarships.h"

struct Param
{
	glm::vec3 GetPos(float u, float v) const noexcept
	{
		return glm::vec3(u, v, 0.0);
	}

	glm::vec3 GetNorm(float u, float v) const noexcept
	{
		return glm::vec3(0.0, 0.0, 1.0);
	}

	glm::vec2 GetTex(float u, float v) const noexcept
	{
		return glm::vec2(u, v);
	}
};

struct Water
{
	float waterWidth;
	float waterHight;

	Water(float _waterWidth, float _waterHight) {
		float waterWidth = _waterWidth;
		float waterHight = _waterHight;
	}

	glm::vec3 GetPos(float u, float v) const noexcept
	{
		glm::vec3 pos = glm::vec3(-10.0, 0.0, 10.0) + glm::vec3(waterWidth, 0.0, -waterHight) * glm::vec3(u, 0.0, v);
		pos.y = sinf((pos.z) / 8.0) + sin((pos.y + pos.x) / 6.0);
		return pos;
	}

	glm::vec3 GetNorm(float u, float v) const noexcept
	{
		glm::vec3 du = GetPos(u + 0.01f, v) - GetPos(u - 0.01f, v);
		glm::vec3 dv = GetPos(u, v + 0.01f) - GetPos(u, v - 0.01f);

		return glm::normalize(glm::cross(du, dv));
	}

	glm::vec2 GetTex(float u, float v) const noexcept
	{
		return glm::vec2(u, v);
	}
};

void WorldOfWarships::InitGeometry()
{
	const std::initializer_list<VertexAttributeDescriptor> vertexAttribList =
	{
		{ 0, offsetof(Vertex, position), 3, GL_FLOAT },
		{ 1, offsetof(Vertex, normal), 3, GL_FLOAT },
		{ 2, offsetof(Vertex, texcoord), 2, GL_FLOAT },
	};

	//OBJ
	MeshObject<Vertex> suzanneMeshCPU = ObjParser::parse("Assets/ship.obj");
	m_SuzanneGPU = CreateGLObjectFromMesh(suzanneMeshCPU, vertexAttribList);

	// Skybox
	InitSkyboxGeometry();

	// Water
	MeshObject<glm::vec2> waterCPU;
	{
		MeshObject<Vertex> surfaceMeshCPU = GetParamSurfMesh(Param(), waterResX, waterResY);
		for (const Vertex& v : surfaceMeshCPU.vertexArray)
		{
			waterCPU.vertexArray.emplace_back(glm::vec2(v.position.x, v.position.y));
		}
		waterCPU.indexArray = surfaceMeshCPU.indexArray;
	}
	m_waterGPU = CreateGLObjectFromMesh(waterCPU, { { 0, offsetof(glm::vec2,x), 2, GL_FLOAT} });
}

void WorldOfWarships::CleanGeometry()
{
	CleanOGLObject(m_SuzanneGPU);
	CleanSkyboxGeometry();
}

void WorldOfWarships::InitSkyboxGeometry()
{
	// skybox geo
	MeshObject<glm::vec3> skyboxCPU =
	{
		std::vector<glm::vec3>
		{
		// hátsó lap
		glm::vec3(-1, -1, -1),
		glm::vec3(1, -1, -1),
		glm::vec3(1,  1, -1),
		glm::vec3(-1,  1, -1),
			// elülsõ lap
			glm::vec3(-1, -1, 1),
			glm::vec3(1, -1, 1),
			glm::vec3(1,  1, 1),
			glm::vec3(-1,  1, 1),
		},

		std::vector<GLuint>
		{
		// hátsó lap
		0, 1, 2,
		2, 3, 0,
			// elülsõ lap
			4, 6, 5,
			6, 4, 7,
			// bal
			0, 3, 4,
			4, 3, 7,
			// jobb
			1, 5, 2,
			5, 6, 2,
			// alsó
			1, 0, 4,
			1, 4, 5,
			// felsõ
			3, 2, 6,
			3, 6, 7,
		}
	};

	m_SkyboxGPU = CreateGLObjectFromMesh(skyboxCPU, { { 0, offsetof(glm::vec3,x), 3, GL_FLOAT } });
}

void WorldOfWarships::CleanSkyboxGeometry()
{
	CleanOGLObject(m_SkyboxGPU);
}