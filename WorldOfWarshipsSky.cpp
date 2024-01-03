/*
 * ELTE IK Számítógépes grafika
 * World of Warships
 * Sándor Balázs
 * AZA6NL
 */

#include "WorldOfWarships.h"

void WorldOfWarships::skyBoxRender() {
	//geom
	glBindVertexArray(skyBoxGeom.vaoID);

	//tex
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);

	//shader
	glUseProgram(shaderSkyBox);

	//mat
	glUniformMatrix4fv(ul("world"), 1, GL_FALSE, glm::value_ptr(glm::translate(camera.GetEye())));
	glUniformMatrix4fv(ul("viewProj"), 1, GL_FALSE, glm::value_ptr(camera.GetViewProj()));

	//tex
	glUniform1i(ul("skyboxTexture"), 0);

	//depth
	GLint prevDepthFnc;
	glGetIntegerv(GL_DEPTH_FUNC, &prevDepthFnc);

	//far
	glDepthFunc(GL_LEQUAL);

	//draw
	glDrawElements(GL_TRIANGLES, skyBoxGeom.count, GL_UNSIGNED_INT, nullptr);
	glDepthFunc(prevDepthFnc);
};