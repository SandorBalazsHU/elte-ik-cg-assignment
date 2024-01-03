/*
 * ELTE IK Számítógépes grafika
 * World of Warships
 * Sándor Balázs
 * AZA6NL
 */

#include <iostream>
#include <cstdlib>
#include <ctime>
#include "WorldOfWarships.h"

struct Blank
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

struct Mountains
{

	float getRandomFloat(float min, float max)  const noexcept {
		return min + static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX / (max - min));
	}

	//https://www.wolframalpha.com/input?i=%28%28sin%28%28%28x-3%2C2%29%5E2%29%2B1%29%2B1%29*%28sin%28y-1%2C1%29%2B1%29%29
	glm::vec3 GetPos(float u, float v) const noexcept
	{
		u *= 6.4f;
		v *= 6.5f;

		glm::vec3 pos = glm::vec3(u, 0.0, v);
		//pos.y = (sinf(u)*sinf(v));
		pos.y = ((sinf(((u - 3.2f) * (u - 3.2f)) + 1.0f) + 1.0f) * (sinf(v - 1.6f) + 1.0f));

		float minValue = -0.05f;
		float maxValue = 0.05f;

		float randomValue = getRandomFloat(minValue, maxValue);
		pos.y += randomValue;

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
		glm::vec3 pos = GetPos(u, v);
		float normalizedY = (pos.y - (-0.5f)) / (4.5f - (-0.5f));
		return glm::vec2(normalizedY, 0.5f);
	}
};

void WorldOfWarships::initGeometry()
{
	const std::initializer_list<VertexAttributeDescriptor> vertexAttribList =
	{
		{ 0, offsetof(Vertex, position), 3, GL_FLOAT },
		{ 1, offsetof(Vertex, normal), 3, GL_FLOAT },
		{ 2, offsetof(Vertex, texcoord), 2, GL_FLOAT },
	};

	//OBJ
	MeshObject<Vertex> shipMesh = ObjParser::parse("Assets/ship.obj");
	shipGeom = CreateGLObjectFromMesh(shipMesh, vertexAttribList);

	MeshObject<Vertex> shipCanonMesh = ObjParser::parse("Assets/ship_canon.obj");
	shipCanonGeom = CreateGLObjectFromMesh(shipCanonMesh, vertexAttribList);

	MeshObject<Vertex> shipTuretMesh = ObjParser::parse("Assets/ship_turet.obj");
	shipTuretGeom = CreateGLObjectFromMesh(shipTuretMesh, vertexAttribList);

	MeshObject<Vertex> lightHouseMesh = ObjParser::parse("Assets/lighthouse.obj");
	lightHouseGeom = CreateGLObjectFromMesh(lightHouseMesh, vertexAttribList);

	// Skybox
	initSkyboxGeometry();

	// Water
	MeshObject<glm::vec2> waterMesh;
	{
		MeshObject<Vertex> surfaceMeshCPU = GetParamSurfMesh(Blank(), waterResX, waterResY);
		for (const Vertex& v : surfaceMeshCPU.vertexArray)
		{
			waterMesh.vertexArray.emplace_back(glm::vec2(v.position.x, v.position.y));
		}
		waterMesh.indexArray = surfaceMeshCPU.indexArray;
	}
	waterGeom = CreateGLObjectFromMesh(waterMesh, { { 0, offsetof(glm::vec2,x), 2, GL_FLOAT} });

	//Mountains
	MeshObject<Vertex> mountainsMesh = GetParamSurfMesh(Mountains(), mountainsResX, mountainsResY);
	mountainsGeom = CreateGLObjectFromMesh(mountainsMesh, vertexAttribList);
}

void WorldOfWarships::cleanGeometry()
{
	CleanOGLObject(shipGeom);
	CleanOGLObject(shipCanonGeom);
	CleanOGLObject(shipTuretGeom);
	CleanOGLObject(lightHouseGeom);
	CleanOGLObject(mountainsGeom);
	cleanSkyboxGeometry();
}

void WorldOfWarships::initSkyboxGeometry()
{
	MeshObject<glm::vec3> skyboxMesh =
	{
		std::vector<glm::vec3>
		{
			//back
			glm::vec3(-1, -1, -1),
			glm::vec3(1, -1, -1),
			glm::vec3(1,  1, -1),
			glm::vec3(-1,  1, -1),
			//front
			glm::vec3(-1, -1, 1),
			glm::vec3(1, -1, 1),
			glm::vec3(1,  1, 1),
			glm::vec3(-1,  1, 1),
		},

		std::vector<GLuint>
		{
		//back
		0, 1, 2,
		2, 3, 0,
			//front
			4, 6, 5,
			6, 4, 7,
			//left
			0, 3, 4,
			4, 3, 7,
			//right
			1, 5, 2,
			5, 6, 2,
			//bottom
			1, 0, 4,
			1, 4, 5,
			//top
			3, 2, 6,
			3, 6, 7,
		}
	};

	skyBoxGeom = CreateGLObjectFromMesh(skyboxMesh, { { 0, offsetof(glm::vec3,x), 3, GL_FLOAT } });
}

void WorldOfWarships::cleanSkyboxGeometry()
{
	CleanOGLObject(skyBoxGeom);
}