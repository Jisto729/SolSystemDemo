#include "SceneObject.h"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

using namespace ssd;

SceneObject::SceneObject(std::vector<float> inVertices, std::vector<float> inColors, std::vector<int> inIndices)
{
	vertices = inVertices;
	colors = inColors;
	indices = inIndices;
}

void SceneObject::moveObject(float dx, float dy, float dz)
{
	modelMatrix = glm::translate(modelMatrix, glm::vec3(dx, dy, dz));
}