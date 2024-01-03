/*
 * ELTE IK Számítógépes grafika
 * World of Warships
 * Sándor Balázs
 * AZA6NL
 */

#include "WorldOfWarships.h"

void WorldOfWarships::lighthouseRender()
{
	glm::mat4 matWorld01 = glm::translate(glm::vec3(500.0f - 20.0f, -1.0f, 500.0f - 20.0f)) * glm::scale(glm::vec3(10.0f, 10.0f, 10.0f));
	drawLighthouse(matWorld01);
	glm::mat4 matWorld02 = glm::translate(glm::vec3(-500.0f + 20.0f, -1.0f, -500.0f + 20.0f)) * glm::scale(glm::vec3(10.0f, 10.0f, 10.0f)) * glm::translate(glm::vec3(-3.2f, 0.0f, -2.9f));
	drawLighthouse(matWorld02);
};

void WorldOfWarships::drawLighthouse(glm::mat4 matWorld)
{
	//geom
	glBindVertexArray(lightHouseGeom.vaoID);

	//text
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, lightHouseTexture);

	//shader
	glUseProgram(shaderBase);

	//mat
	glUniformMatrix4fv(ul("world"), 1, GL_FALSE, glm::value_ptr(matWorld));
	glUniformMatrix4fv(ul("worldIT"), 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(matWorld))));
	glUniformMatrix4fv(ul("viewProj"), 1, GL_FALSE, glm::value_ptr(camera.GetViewProj()));

	//light
	setLights();

	//text
	glUniform1i(ul("texImage"), 0);

	//draw
	glDrawElements(GL_TRIANGLES,
		lightHouseGeom.count,
		GL_UNSIGNED_INT,
		nullptr);
};