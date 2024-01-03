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
	float ElapsedTimeInSec = 0.0f; // Program indulása óta eltelt idő
	float DeltaTimeInSec   = 0.0f; // Előző Update óta eltelt idő
};

class WorldOfWarships
{
public:
	WorldOfWarships();
	~WorldOfWarships();

	bool Init();
	void Clean();

	void Update( const SUpdateInfo& );
	void Render();
	void RenderGUI();

	void keyboardDown(const SDL_KeyboardEvent&);
	void keyboardUp(const SDL_KeyboardEvent&);
	void mouseMove(const SDL_MouseMotionEvent&);
	void mouseDown(const SDL_MouseButtonEvent&);
	void mouseUp(const SDL_MouseButtonEvent&);
	void mouseWheel(const SDL_MouseWheelEvent&);
	void resize(int, int);
protected:

	float m_ElapsedTimeInSec = 0.0f;

	// Kamera
	Camera m_camera;

	//
	// OpenGL-es dolgok
	//
	
	// uniform location lekérdezése
	GLint ul( const char* uniformName ) noexcept;

	// shaderekhez szükséges változók
	GLuint m_programID = 0;		  // shaderek programja
	GLuint m_programSkyboxID = 0; // skybox programja
	GLuint m_programWaterID = 0;   // viz programja
	GLuint m_programAxesID = 0;   // koordinata tengelyek programja
	GLuint m_programTrajectoryID = 0;   // utvonal programja

	// Fényforrás- ...
	glm::vec4 m_lightPos = glm::vec4( 0.0f, 1.0f, 0.0f, 0.0f );
	glm::vec3 m_La = glm::vec3( 0.125f );
	glm::vec3 m_Ld = glm::vec3(1.0, 1.0, 1.0 );
	glm::vec3 m_Ls = glm::vec3(1.0, 1.0, 1.0 );

	float m_lightConstantAttenuation    = 1.0;
	float m_lightLinearAttenuation      = 0.0;
	float m_lightQuadraticAttenuation   = 0.0;

	// ... és anyagjellemzők
	glm::vec3 m_Ka = glm::vec3( 1.0 );
	glm::vec3 m_Kd = glm::vec3( 1.0 );
	glm::vec3 m_Ks = glm::vec3( 1.0 );

	float m_Shininess = 1.0;

	// Shaderek inicializálása, és törtlése
	void InitShaders();
	void CleanShaders();
	void InitSkyboxShaders();
	void CleanSkyboxShaders();

	// Geometriával kapcsolatos változók

	OGLObject m_SuzanneGPU = {};
	OGLObject m_SkyboxGPU = {};
	OGLObject m_waterGPU = {};
	OGLObject m_quadGPU = {};

	// Geometria inicializálása, és törtlése
	void InitGeometry();
	void CleanGeometry();
	void InitSkyboxGeometry();
	void CleanSkyboxGeometry();

	// Textúrázás, és változói

	GLuint m_SuzanneTextureID = 0;
	GLuint m_skyboxTextureID = 0;
	GLuint m_waterTextureID = 0;
	GLuint m_woodNormalMapTextureID = 0;
	GLuint m_glassTextureID = 0;

	void InitTextures();
	void CleanTextures();
	void InitSkyboxTextures();
	void CleanSkyboxTextures();
	void setupDebugCallback();
};