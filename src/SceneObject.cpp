#include "SceneObject.h"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

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
}

//TODO add movement based on type and parameters of object(planet, moon, ...)