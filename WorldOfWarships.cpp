/*
 * ELTE IK Számítógépes grafika
 * World of Warships
 * Sándor Balázs
 * AZA6NL
 */

#include "WorldOfWarships.h"

WorldOfWarships::WorldOfWarships() {};
WorldOfWarships::~WorldOfWarships() {};

bool WorldOfWarships::init()
{
	setupDebugCallback();
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);
	GLfloat pointSizeRange[2] = { 0.0f, 0.0f };
	glGetFloatv(GL_POINT_SIZE_RANGE, pointSizeRange);
	glPointSize(std::min(16.0f, pointSizeRange[1]));

	initShaders();
	initGeometry();
	initTextures();

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);

	camera.SetView(glm::vec3(0.0, 40.0, 25.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	return true;
};

void WorldOfWarships::clean()
{
	cleanShaders();
	cleanGeometry();
	cleanTextures();
};

void WorldOfWarships::update(const SUpdateInfo& updateInfo)
{
	elapsedTimeInSec = updateInfo.ElapsedTimeInSec;
	camera.Update(updateInfo.DeltaTimeInSec);
};

void WorldOfWarships::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	sceneRender();
	mountainsRender();
	lighthouseRender();
	waterRender();
	skyBoxRender();
	renderClean();
};

void WorldOfWarships::renderClean() {
	glUseProgram(0);

	glActiveTexture(GL_TEXTURE0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	glBindVertexArray(0);
};

void WorldOfWarships::renderGUI()
{
};