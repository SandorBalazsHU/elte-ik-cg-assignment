/*
 * ELTE IK Számítógépes grafika
 * World of Warships
 * Sándor Balázs
 * AZA6NL
 */

#include "WorldOfWarships.h"

void WorldOfWarships::sceneRender() {
	drawSceneObject(shipGeom, shipTexture);
	drawSceneObject(shipCanonGeom, shipCanonTexture);
	drawSceneObject(shipTuretGeom, shipTuretTexture);
}

void WorldOfWarships::drawSceneObject(OGLObject &geom, GLuint &texture) {

	//geom
	glBindVertexArray(geom.vaoID);

	//text
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	//shader
	glUseProgram(shaderBase);

	//transform
	glm::vec3 pos = glm::vec3(0.0);
	pos.y = sin((pos.z + elapsedTimeInSec) / 8.0) + sin((pos.y + pos.x + elapsedTimeInSec) / 6.0);
	glm::mat4 matWorld = glm::translate(pos);

	//mat
	glUniformMatrix4fv(ul("world"), 1, GL_FALSE, glm::value_ptr(matWorld));
	glUniformMatrix4fv(ul("worldIT"), 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(matWorld))));
	glUniformMatrix4fv(ul("viewProj"), 1, GL_FALSE, glm::value_ptr(camera.GetViewProj()));

	//light
	glUniform3fv(ul("cameraPos"), 1, glm::value_ptr(camera.GetEye()));
	glUniform4fv(ul("lightPos"), 1, glm::value_ptr(m_lightPos));
	glUniform3fv(ul("La"), 1, glm::value_ptr(m_La));
	glUniform3fv(ul("Ld"), 1, glm::value_ptr(m_Ld));
	glUniform3fv(ul("Ls"), 1, glm::value_ptr(m_Ls));
	glUniform1f(ul("lightConstantAttenuation"), m_lightConstantAttenuation);
	glUniform1f(ul("lightLinearAttenuation"), m_lightLinearAttenuation);
	glUniform1f(ul("lightQuadraticAttenuation"), m_lightQuadraticAttenuation);

	//material
	glUniform3fv(ul("Ka"), 1, glm::value_ptr(m_Ka));
	glUniform3fv(ul("Kd"), 1, glm::value_ptr(m_Kd));
	glUniform3fv(ul("Ks"), 1, glm::value_ptr(m_Ks));

	//gloss
	glUniform1f(ul("Shininess"), m_Shininess);

	//tex
	glUniform1i(ul("texImage"), 0);

	//draw
	glDrawElements(GL_TRIANGLES,
		geom.count,
		GL_UNSIGNED_INT,
		nullptr);
};