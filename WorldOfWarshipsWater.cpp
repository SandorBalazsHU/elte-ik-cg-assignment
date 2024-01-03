/*
 * ELTE IK Számítógépes grafika
 * World of Warships
 * Sándor Balázs
 * AZA6NL
 */

#include "WorldOfWarships.h"

void WorldOfWarships::waterRender() {
	//geom
	glBindVertexArray(waterGeom.vaoID);

	//text
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, waterTexture);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, waterNormalMapTexture);

	//shader
	glUseProgram(shaderWater);

	//transform
	glm::mat4 matWorld = glm::translate(glm::vec3(0.0, 0.1, 0.0));

	//mat
	glUniformMatrix4fv(ul("world"), 1, GL_FALSE, glm::value_ptr(matWorld));
	glUniformMatrix4fv(ul("worldIT"), 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(matWorld))));
	glUniformMatrix4fv(ul("viewProj"), 1, GL_FALSE, glm::value_ptr(camera.GetViewProj()));

	//light
	setLights();

	//shader parameters
	glUniform1f(ul("elapsedTimeInSec"), elapsedTimeInSec);
	glUniform1f(ul("waterWidth"), waterWidth);
	glUniform1f(ul("waterHight"), waterHight);

	//text
	glUniform1i(ul("texImage"), 0);
	glUniform1i(ul("texNormalMap"), 1);

	//draw
	glDrawElements(GL_TRIANGLES,
		waterGeom.count,
		GL_UNSIGNED_INT,
		nullptr);
};