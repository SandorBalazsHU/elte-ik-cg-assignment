/*
 * ELTE IK Számítógépes grafika
 * World of Warships
 * Sándor Balázs
 * AZA6NL
 */

#include "WorldOfWarships.h"
#include "WorldOfWarshipsLights.h"

void WorldOfWarships::setLights() {
	glUniform3fv(ul("cameraPos"), 1, glm::value_ptr(camera.GetEye()));

	//light00
	glUniform4fv(ul("lightPos_00"), 1, glm::value_ptr(lightPos_00));
	glUniform3fv(ul("lightAmbient_00"), 1, glm::value_ptr(lightAmbient_00));
	glUniform3fv(ul("lightDiffuse_00"), 1, glm::value_ptr(lightDiffuse_00));
	glUniform3fv(ul("lightSpecular_00"), 1, glm::value_ptr(lightSpecular_00));
	glUniform1f(ul("lightConstantAttenuation_00"), lightConstantAttenuation_00);
	glUniform1f(ul("lightLinearAttenuation_00"), lightLinearAttenuation_00);
	glUniform1f(ul("lightQuadraticAttenuation_00"), lightQuadraticAttenuation_00);

	glUniform3fv(ul("materialAmbient_00"), 1, glm::value_ptr(materialAmbient_00));
	glUniform3fv(ul("materialDiffuse_00"), 1, glm::value_ptr(materialDiffuse_00));
	glUniform3fv(ul("materialSpecular_00"), 1, glm::value_ptr(materialSpecular_00));

	glUniform1f(ul("hininess_00"), shininess_00);

	//light01
	glUniform4fv(ul("lightPos_01"), 1, glm::value_ptr(lightPos_00));
	glUniform3fv(ul("lightAmbient_01"), 1, glm::value_ptr(lightAmbient_01));
	glUniform3fv(ul("lightDiffuse_01"), 1, glm::value_ptr(lightDiffuse_01));
	glUniform3fv(ul("lightSpecular_01"), 1, glm::value_ptr(lightSpecular_01));
	glUniform1f(ul("lightConstantAttenuation_01"), lightConstantAttenuation_01);
	glUniform1f(ul("lightLinearAttenuation_01"), lightLinearAttenuation_01);
	glUniform1f(ul("lightQuadraticAttenuation_01"), lightQuadraticAttenuation_01);

	glUniform3fv(ul("materialAmbient_01"), 1, glm::value_ptr(materialAmbient_01));
	glUniform3fv(ul("materialDiffuse_01"), 1, glm::value_ptr(materialDiffuse_01));
	glUniform3fv(ul("materialSpecular_01"), 1, glm::value_ptr(materialSpecular_01));

	glUniform1f(ul("hininess_01"), shininess_01);

	//light02
	glUniform4fv(ul("lightPos_02"), 1, glm::value_ptr(lightPos_02));
	glUniform3fv(ul("lightAmbient_02"), 1, glm::value_ptr(lightAmbient_02));
	glUniform3fv(ul("lightDiffuse_02"), 1, glm::value_ptr(lightDiffuse_02));
	glUniform3fv(ul("lightSpecular_02"), 1, glm::value_ptr(lightSpecular_02));
	glUniform1f(ul("lightConstantAttenuation_02"), lightConstantAttenuation_02);
	glUniform1f(ul("lightLinearAttenuation_02"), lightLinearAttenuation_02);
	glUniform1f(ul("lightQuadraticAttenuation_02"), lightQuadraticAttenuation_02);

	glUniform3fv(ul("materialAmbient_02"), 1, glm::value_ptr(materialAmbient_02));
	glUniform3fv(ul("materialDiffuse_02"), 1, glm::value_ptr(materialDiffuse_02));
	glUniform3fv(ul("materialSpecular_02"), 1, glm::value_ptr(materialSpecular_02));

	glUniform1f(ul("hininess_02"), shininess_02);
}