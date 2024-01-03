/*
 * ELTE IK Számítógépes grafika
 * World of Warships
 * Sándor Balázs
 * AZA6NL
 */

#pragma once

#include <string>
#include <vector>
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

struct Ship
{
	Ship() : position(glm::vec3(0)), velocity(glm::vec3(0)), turetAngle(0), canonAngle(0), hp(100) {};
	glm::vec3 position;
	glm::vec3 velocity;
	float turetAngle;
	float canonAngle;
	int hp;
	int team;
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
	
	//Ships
	int numberOfallyShips = 5;
	int numberOfenemyShips = 5;
	std::vector<Ship> allyShips;
	std::vector<Ship> enemyShips;
	int playerShipID = 2;
	float shipAcceleration = 1.5f;
	float shipSlowing = 0.9f;
	glm::vec3 startingVelocity = glm::vec3(0.01f, 0.0f, 0.0f);
	float leftVelocity = 1.0f;
	float rightVelocity = -1.0f;

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
	bool camIsSticked = true;

	void initScene();
	void rotateTuret(float rotation, Ship &currentShip);
	void rotateCanon(float rotation, Ship &currentShip);
	void shipAccelerate(Ship &currentShip);
	void shipSlow(Ship &currentShip);
	void shipRotateLeft(Ship &currentShip);
	void shipRotateRight(Ship &currentShip);
	void updateScene();
	glm::mat4 rotateShip(Ship &currentShip);

	void setLights();

	void sceneRender();
	void drawSceneObject(glm::vec3 pos, glm::mat4 rotation, OGLObject& geom, GLuint& texture);
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