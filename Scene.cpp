/*
 * ELTE IK Számítógépes grafika
 * World of Warships
 * Sándor Balázs
 * AZA6NL
 */

#include "WorldOfWarships.h"

void WorldOfWarships::initScene() {
	float const fleatDistance = 50.0f;
	float const shipDistance = 50.0f;
	float const shipShiftDistance = 5.0f;

	for (size_t i = 0; i < numberOfallyShips; i++)
	{
		Ship newShip;
		newShip.team = 1;
		newShip.position.x -= fleatDistance;
		newShip.position.z -= ((numberOfallyShips * shipDistance)/2.0f) - (i * shipDistance);
		newShip.velocity = startingVelocity;
		allyShips.push_back(newShip);
	}

	for (size_t i = 0; i < numberOfenemyShips; i++)
	{
		Ship newShip;
		newShip.team = 2;
		newShip.position.x -= fleatDistance;
		newShip.position.z -= ((numberOfallyShips * shipDistance) / 2.0f) - (i * shipDistance);
		newShip.position.z -= shipShiftDistance;
		newShip.velocity = startingVelocity;
		enemyShips.push_back(newShip);
	}
	//allyShips[playerShipID].velocity = startingVelocity;
};

void  WorldOfWarships::rotateTuret(float rotation, Ship &currentShip) {
	currentShip.turetAngle += rotation;
};

void  WorldOfWarships::rotateCanon(float rotation, Ship &currentShip) {
	if (
		((currentShip.canonAngle + rotation) >= -20.0f) &&
		((currentShip.canonAngle + rotation) <= 60.0f)
		)
		currentShip.canonAngle += rotation;
};

void WorldOfWarships::shipAccelerate(Ship &currentShip) {
	//if (!(currentShip.velocity.x = 0 && currentShip.velocity.z == 0)) currentShip.velocity = startingVelocity;
	currentShip.velocity *= shipAcceleration;
};

void WorldOfWarships::shipSlow(Ship &currentShip) {
	currentShip.velocity *= shipSlowing;
};

void WorldOfWarships::shipRotateLeft(Ship &currentShip) {
	glm::mat4 shipRotation = glm::rotate(glm::radians(leftVelocity), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::vec3 rotatedVector = glm::vec3(shipRotation * glm::vec4(currentShip.velocity, 1.0f));
	currentShip.velocity = rotatedVector;
};

void WorldOfWarships::shipRotateRight(Ship &currentShip) {
	glm::mat4 shipRotation = glm::rotate(glm::radians(rightVelocity), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::vec3 rotatedVector = glm::vec3(shipRotation * glm::vec4(currentShip.velocity, 1.0f));
	currentShip.velocity = rotatedVector;
};

void WorldOfWarships::updateScene() {
	for (size_t i = 0; i < numberOfallyShips; i++)
	{
		allyShips[i].position += allyShips[i].velocity;
	}

	for (size_t i = 0; i < numberOfenemyShips; i++)
	{
		//enemyShips[i].position += enemyShips[i].velocity;
	}
}