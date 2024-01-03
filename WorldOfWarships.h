/*
 * ELTE IK Számítógépes grafika
 * World of Warships
 * Sándor Balázs
 * AZA6NL
 */

#pragma once

#include <string>
#include <imgui.h>
#include "Camera.h"
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include "ObjParser.h"
#include "GLUtils.hpp"
#include <glm/glm.hpp>
#include <SDL2/SDL_opengl.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform2.hpp>
#include "ParametricSurfaceMesh.hpp"
#include "SDL_GLDebugMessageCallback.h"
#include <glm/gtc/matrix_transform.hpp>

struct SUpdateInfo
{
	float ElapsedTimeInSec = 0.0f;
	float DeltaTimeInSec   = 0.0f;
};

class WorldOfWarships
{
public:
	WorldOfWarships();
	~WorldOfWarships();

	bool init();
	void clean();

	void update( const SUpdateInfo& );
	void render();
	void renderGUI();

	void keyboardDown(const SDL_KeyboardEvent&);
	void keyboardUp(const SDL_KeyboardEvent&);
	void mouseMove(const SDL_MouseMotionEvent&);
	void mouseDown(const SDL_MouseButtonEvent&);
	void mouseUp(const SDL_MouseButtonEvent&);
	void mouseWheel(const SDL_MouseWheelEvent&);
	void resize(int, int);

private:
	//water
	float waterWidth = 1000.0f;
	float waterHight = 1000.0f;
	int waterResX = 1000;
	int waterResY = 1000;
	float elapsedTimeInSec = 0.0f;

	//mountains
	float mountainsWidth = 20.0f;
	float mountainsHight = 20.0f;
	int mountainsResX = 100;
	int mountainsResY = 100;

	//shaders
	GLuint shaderBase = 0;
	GLuint shaderSkyBox = 0;
	GLuint shaderWater = 0;
	GLuint mountainsShader = 0;

	//geom
	OGLObject shipGeom = {};
	OGLObject shipCanonGeom = {};
	OGLObject shipTuretGeom = {};
	OGLObject lightHouseGeom = {};
	OGLObject skyBoxGeom = {};
	OGLObject waterGeom = {};
	OGLObject mountainsGeom = {};


	//textures
	GLuint shipTexture = 0;
	GLuint shipTuretTexture = 0;
	GLuint shipCanonTexture = 0;
	GLuint lightHouseTexture = 0;
	GLuint skyboxTexture = 0;
	GLuint waterTexture = 0;
	GLuint waterNormalMapTexture = 0;
	GLuint mountainsTexture = 0;

	//cam
	Camera camera;

	//light
	glm::vec4 m_lightPos = glm::vec4( 0.0f, 1.0f, 0.0f, 0.0f );
	glm::vec3 m_La = glm::vec3( 0.125f );
	glm::vec3 m_Ld = glm::vec3(1.0, 1.0, 1.0 );
	glm::vec3 m_Ls = glm::vec3(1.0, 1.0, 1.0 );
	float m_lightConstantAttenuation    = 1.0;
	float m_lightLinearAttenuation      = 0.0;
	float m_lightQuadraticAttenuation   = 0.0;

	//material
	glm::vec3 m_Ka = glm::vec3( 1.0 );
	glm::vec3 m_Kd = glm::vec3( 1.0 );
	glm::vec3 m_Ks = glm::vec3( 1.0 );
	float m_Shininess = 1.0;

	void sceneRender();
	void drawSceneObject(OGLObject& geom, GLuint& texture);
	void waterRender();
	void skyBoxRender();
	void mountainsRender();
	void drawMountain(glm::mat4 matWorld);
	void lighthouseRender();
	void drawLighthouse(glm::mat4 matWorld);
	void renderClean();

	void initTextures();
	void cleanTextures();
	void initShaders();
	void cleanShaders();
	void initGeometry();
	void cleanGeometry();
	void initSkyboxGeometry();
	void cleanSkyboxGeometry();
	void setupDebugCallback();

	GLint ul(const char* uniformName) noexcept;
};