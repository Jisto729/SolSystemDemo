#include "SunSceneObject.h"

#include <iostream>

using namespace ssd;

SunSceneObject::SunSceneObject(std::vector<float> inVertices, std::vector<float> inColors, std::vector<int> inIndices, glm::vec3 inCenter, ObjectType inType)
{
	vertices = inVertices;
	colors = inColors;
	indices = inIndices;
	center = inCenter;
	type = inType;
	texture = std::make_shared<ssl::SunTexture>(10);
}

std::shared_ptr<ssl::SunTexture> SunSceneObject::getTexture() 
{ 
	return texture; 
}

void SunSceneObject::animateTexture()
{
	texture->nextFrame();
}