/*
 * ELTE IK Számítógépes grafika
 * World of Warships
 * Sándor Balázs
 * AZA6NL
 */

#include "WorldOfWarships.h"

void WorldOfWarships::sceneRender() {
	for (size_t i = 0; i < numberOfallyShips; i++)
	{
		Ship currentShip = allyShips[i];
		glm::mat4 shipRotation = rotateShip(currentShip);

		//glm::mat4 shipRotation = glm::rotate(glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		drawSceneObject(currentShip.position, shipRotation, shipGeom, shipTexture);

		glm::mat4 turetRotation = shipRotation * glm::rotate(glm::radians(currentShip.turetAngle), glm::vec3(0.0f, 1.0f, 0.0f));
		drawSceneObject(currentShip.position, turetRotation, shipTuretGeom, shipTuretTexture);

		glm::mat4 canonRotation = turetRotation * glm::rotate(glm::radians(currentShip.canonAngle), glm::vec3(1.0f, 0.0f, 0.0f));
		drawSceneObject(currentShip.position, canonRotation, shipCanonGeom, shipCanonTexture);

	}

	for (size_t i = 0; i < numberOfenemyShips; i++)
	{
		Ship currentShip = enemyShips[i];
		//glm::mat4 shipRotation = rotateShip(currentShip);

		glm::mat4 shipRotation = glm::rotate(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		drawSceneObject(currentShip.position, shipRotation, shipGeom, shipTexture);

		glm::mat4 turetRotation = shipRotation * glm::rotate(glm::radians(50.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		drawSceneObject(currentShip.position, turetRotation, shipTuretGeom, shipTuretTexture);

		glm::mat4 canonRotation = turetRotation * glm::rotate(glm::radians(20.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		drawSceneObject(currentShip.position, canonRotation, shipCanonGeom, shipCanonTexture);
	}

}

void WorldOfWarships::drawSceneObject(glm::vec3 pos, glm::mat4 rotation, OGLObject &geom, GLuint &texture) {

	//geom
	glBindVertexArray(geom.vaoID);

	//text
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	//shader
	glUseProgram(shaderBase);

	//transform
	pos.y = sin((pos.z + elapsedTimeInSec) / 8.0) + sin((pos.y + pos.x + elapsedTimeInSec) / 6.0);
	glm::mat4 matWorld = glm::translate(pos) * rotation;

	//mat
	glUniformMatrix4fv(ul("world"), 1, GL_FALSE, glm::value_ptr(matWorld));
	glUniformMatrix4fv(ul("worldIT"), 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(matWorld))));
	glUniformMatrix4fv(ul("viewProj"), 1, GL_FALSE, glm::value_ptr(camera.GetViewProj()));

	//light
	setLights();

	//tex
	glUniform1i(ul("texImage"), 0);

	//draw
	glDrawElements(GL_TRIANGLES,
		geom.count,
		GL_UNSIGNED_INT,
		nullptr);
};

glm::mat4 WorldOfWarships::rotateShip(Ship &currentShip)
{	
	glm::vec3 suzanneForward;
	if (currentShip.velocity.x == 0 && currentShip.velocity.z == 0) suzanneForward =  glm::vec3(1.0, 0.0, 0.0);
	glm::vec3 nextPoint = currentShip.position + currentShip.velocity;
	suzanneForward = glm::normalize(nextPoint - currentShip.position);

	glm::vec3 suzanneRight = glm::normalize(glm::cross(suzanneForward, glm::vec3(0.0, 1.0, 0.0)));
	glm::vec3 suzanneUp = glm::cross(suzanneRight, suzanneForward);

	glm::mat4 suzanneRot(1.0f);
	suzanneRot[0] = glm::vec4(suzanneForward, 0.0f);
	suzanneRot[1] = glm::vec4(suzanneUp, 0.0f);
	suzanneRot[2] = glm::vec4(suzanneRight, 0.0f);

	static const glm::mat4 suzanneTowardX = glm::rotate(glm::radians(270.0f), glm::vec3(0.0, 1.0, 0.0));

	return suzanneRot * suzanneTowardX;
}