#pragma once

//light00
glm::vec4 lightPos_00 = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);

glm::vec3 lightAmbient_00 = glm::vec3(0.125f);
glm::vec3 lightDiffuse_00 = glm::vec3(1.0, 1.0, 1.0);
glm::vec3 lightSpecular_00 = glm::vec3(1.0, 1.0, 1.0);
float lightConstantAttenuation_00 = 1.0;
float lightLinearAttenuation_00 = 0.0;
float lightQuadraticAttenuation_00 = 0.0;

glm::vec3 materialAmbient_00 = glm::vec3(1.0);
glm::vec3 materialDiffuse_00 = glm::vec3(1.0);
glm::vec3 materialSpecular_00 = glm::vec3(1.0);

float shininess_00 = 1.0;

//light01
glm::vec4 lightPos01 = glm::vec4(500.0f - 20.0f, 5.0f, 500.0f - 20.0f, 1.0);

glm::vec4 lightPos_01 = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);

glm::vec3 lightAmbient_01 = glm::vec3(0.125f);
glm::vec3 lightDiffuse_01 = glm::vec3(1.0, 0.0, 0.0);
glm::vec3 lightSpecular_01 = glm::vec3(1.0, 0.0, 0.0);
float lightConstantAttenuation_01 = 1.0;
float lightLinearAttenuation_01 = 0.5;
float lightQuadraticAttenuation_01 = 0.5;

glm::vec3 materialAmbient_01 = glm::vec3(1.0);
glm::vec3 materialDiffuse_01 = glm::vec3(1.0);
glm::vec3 materialSpecular_01 = glm::vec3(1.0);

float shininess_01 = 1.0;

//light02
glm::vec4 lightPos02 = glm::vec4(500.0f - 20.0f, 5.0f, 500.0f - 20.0f, 1.0);

glm::vec4 lightPos_02 = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);

glm::vec3 lightAmbient_02 = glm::vec3(0.125f);
glm::vec3 lightDiffuse_02 = glm::vec3(0.0, 0.0, 1.0);
glm::vec3 lightSpecular_02 = glm::vec3(0.0, 0.0, 1.0);
float lightConstantAttenuation_02 = 1.0;
float lightLinearAttenuation_02 = 0.5;
float lightQuadraticAttenuation_02 = 0.5;

glm::vec3 materialAmbient_02 = glm::vec3(1.0);
glm::vec3 materialDiffuse_02 = glm::vec3(1.0);
glm::vec3 materialSpecular_02 = glm::vec3(1.0);

float shininess_02 = 1.0;