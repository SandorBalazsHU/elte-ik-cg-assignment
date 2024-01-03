/*
 * ELTE IK Számítógépes grafika
 * World of Warships
 * Sándor Balázs
 * AZA6NL
 */

#include "WorldOfWarships.h"
void WorldOfWarships::mountainsRender()
{
	glm::mat4 matWorld01 = glm::translate(glm::vec3(0.0f, -1.0f, 500.0f - 20.0f)) * glm::scale(glm::vec3(130.0f, 20.0f, 20.0f)) * glm::translate(glm::vec3(-3.2f, 0.0f, -2.9f));
	drawMountain(matWorld01);
	glm::mat4 matWorld02 = glm::translate(glm::vec3(0.0f, -1.0f, -500.0f + 20.0f)) * glm::scale(glm::vec3(130.0f, 20.0f, 20.0f)) * glm::translate(glm::vec3(-3.2f, 0.0f, -2.9f));
	drawMountain(matWorld02);
	glm::mat4 matWorld03 = glm::translate(glm::vec3(500.0f - 20.0f, -1.0f, 0.0f))
		* (glm::rotate(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * 
			(glm::scale(glm::vec3(130.0f, 20.0f, 20.0f)) * glm::translate(glm::vec3(-3.2f, 0.0f, -2.9f))));
	drawMountain(matWorld03);
	glm::mat4 matWorld04 = glm::translate(glm::vec3(-500.0f + 20.0f, -1.0f, 0.0f))
		* (glm::rotate(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * 
			(glm::scale(glm::vec3(130.0f, 20.0f, 20.0f)) * glm::translate(glm::vec3(-3.2f, 0.0f, -2.9f))));
	drawMountain(matWorld04);
};

void WorldOfWarships::drawMountain(glm::mat4 matWorld)
{
	//disable baceók
	glDisable(GL_CULL_FACE);

	//geom
	glBindVertexArray(mountainsGeom.vaoID);

	//text
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mountainsTexture);

	//shader
	glUseProgram(mountainsShader);

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
	glUniform1f(ul("mountainsWidth"), mountainsWidth);
	glUniform1f(ul("mountainsHight"), mountainsHight);

	//text
	glUniform1i(ul("texImage"), 0);

	//draw
	glDrawElements(GL_TRIANGLES,
		mountainsGeom.count,
		GL_UNSIGNED_INT,
		nullptr);

	//enable back
	glEnable(GL_CULL_FACE);
};