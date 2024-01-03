/*
 * ELTE IK Számítógépes grafika
 * World of Warships
 * Sándor Balázs
 * AZA6NL
 */

#include "WorldOfWarships.h"

void WorldOfWarships::waterRender() {
	//disable baceók
	//glDisable(GL_CULL_FACE);

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
	glUniform3fv(ul("cameraPos"), 1, glm::value_ptr(camera.GetEye()));
	glUniform4fv(ul("lightPos"), 1, glm::value_ptr(m_lightPos));
	glUniform3fv(ul("La"), 1, glm::value_ptr(m_La));
	glUniform3fv(ul("Ld"), 1, glm::value_ptr(m_Ld));
	glUniform3fv(ul("Ls"), 1, glm::value_ptr(m_Ls));
	glUniform1f(ul("lightConstantAttenuation"), m_lightConstantAttenuation);
	glUniform1f(ul("lightLinearAttenuation"), m_lightLinearAttenuation);
	glUniform1f(ul("lightQuadraticAttenuation"), m_lightQuadraticAttenuation);

	//mat
	glUniform3fv(ul("Ka"), 1, glm::value_ptr(m_Ka));
	glUniform3fv(ul("Kd"), 1, glm::value_ptr(m_Kd));
	glUniform3fv(ul("Ks"), 1, glm::value_ptr(m_Ks));

	//gloss
	glUniform1f(ul("Shininess"), m_Shininess);

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

	//enable back
	//glEnable(GL_CULL_FACE);
};