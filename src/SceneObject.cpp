#include "SceneObject.h"

#include <iostream>

using namespace ssd;

SceneObject::SceneObject(std::vector<float> inVertices, std::vector<float> inColors, std::vector<int> inIndices)
{
	vertices = inVertices;
	colors = inColors;
	indices = inIndices;
}

void SceneObject::moveObject(float dx, float dy, float dz)
{
	if (vertices.size() % 3 != 0) {
		return;
	}
	for (int i = 0; i < vertices.size(); i += 3) {
		vertices[i] += dx;
		vertices[i + 1] += dy;
		vertices[i + 2] += dz;
	}
}