#include "SceneObject.h"

using namespace ssd;

SceneObject::SceneObject(std::vector<float> inVertices, std::vector<float> inColors, std::vector<int> inIndices) {
	vertices = inVertices;
	colors = inColors;
	indices = inIndices;
}