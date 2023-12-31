#include "SceneObject.h"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

using namespace ssd;

SceneObject::SceneObject(std::vector<float> inVertices, std::vector<float> inColors, std::vector<int> inIndices, glm::vec3 inCenter)
{
	vertices = inVertices;
	colors = inColors;
	indices = inIndices;
	center = inCenter;
}

void SceneObject::moveObject(float dx, float dy, float dz)
{
	modelMatrix = glm::translate(modelMatrix, glm::vec3(dx, dy, dz));
}

void SceneObject::revolveAroundPoint(glm::vec3 rotCenter, float angle)
{
	glm::vec3 origin = glm::vec3(0.0, 0.0, 0.0);
	glm::vec3 translateVec = rotCenter - origin;
	modelMatrix = glm::translate(glm::rotate(glm::translate(modelMatrix, translateVec), angle, glm::vec3(0.0f, 1.0f, 0.0f)), -translateVec);
	//TODO change center location based on the rotation
	glm::mat4 rotateCenter = glm::translate(glm::rotate(glm::translate(glm::mat4(1.0f), translateVec), angle, glm::vec3(0.0f, 1.0f, 0.0f)), -translateVec);
	center = glm::vec3(rotateCenter * glm::vec4(center, 1.0f));
	//std::cout << glm::to_string(center) << std::endl;
}

//moves an object around a center, while also rotating itself
void SceneObject::movePlanet(glm::vec3 rotCenter, float rotAngle, float orbitAngle)
{
	glm::vec3 origin = glm::vec3(0.0, 0.0, 0.0);

	glm::mat4 rotateCenter = glm::rotate(glm::mat4(1.0f), orbitAngle, glm::vec3(0.0f, 1.0f, 0.0f));
	center = glm::vec3(rotateCenter * glm::vec4(center, 1.0f));

	glm::vec3 translateToOrigin = center - origin;

	//moving the planet to origin to rotate around its axis
	glm::mat4 moveToOrigin = glm::translate(glm::mat4(1.0f), translateToOrigin);
	//rotate around axis
	glm::mat4 RotateAxis = glm::rotate(glm::mat4(1.0f), rotAngle, glm::vec3(0.0f, 1.0f, 0.0f));
	//move back
	glm::mat4 moveBack = glm::translate(glm::mat4(1.0f), -translateToOrigin);
	//rotate in orbit
	glm::mat4 RotateOrbit = glm::rotate(glm::mat4(1.0f), orbitAngle, glm::vec3(0.0f, 1.0f, 0.0f));

	//updating model matrix
	modelMatrix = moveToOrigin * RotateAxis * moveBack * RotateOrbit * modelMatrix;
}

void SceneObject::moveToPosition(glm::vec3 position)
{
	modelMatrix = glm::translate(glm::mat4(1.0f), position);
}
//TODO add movement based on type and parameters of object(planet, moon, ...)