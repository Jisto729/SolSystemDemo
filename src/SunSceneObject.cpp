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
	texture = std::make_shared<ssl::SunTexture>(10, 10000);
	textureObject = std::make_shared<ge::gl::Texture>(GL_TEXTURE_CUBE_MAP, GL_RGB16F, 0, texture->getWidth(), texture->getHeight());
}

void SunSceneObject::bindCurrentTexture()
{
	std::vector<std::vector<GLfloat>> texVector = texture->getCurrentTexture();

	textureObject->bind(textureObject->getId());

	textureObject->setData2D(texVector[0].data(), GL_RGB, GL_FLOAT, 0, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, 0, texture->getWidth(), texture->getHeight(), 0);
	textureObject->setData2D(texVector[1].data(), GL_RGB, GL_FLOAT, 0, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, 0, texture->getWidth(), texture->getHeight(), 0);
	textureObject->setData2D(texVector[2].data(), GL_RGB, GL_FLOAT, 0, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, 0, texture->getWidth(), texture->getHeight(), 0);
	textureObject->setData2D(texVector[3].data(), GL_RGB, GL_FLOAT, 0, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, 0, texture->getWidth(), texture->getHeight(), 0);
	textureObject->setData2D(texVector[5].data(), GL_RGB, GL_FLOAT, 0, GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, 0, texture->getWidth(), texture->getHeight(), 0);
	textureObject->setData2D(texVector[4].data(), GL_RGB, GL_FLOAT, 0, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, 0, texture->getWidth(), texture->getHeight(), 0);
	textureObject->generateMipmap();
}

void SunSceneObject::unbindCurrentTexture()
{
	textureObject->unbind(textureObject->getId());
}

void SunSceneObject::regenerateTexture(int seed, int temperature)
{
	texture = std::make_shared<ssl::SunTexture>(seed, temperature);
}

std::shared_ptr<ssl::SunTexture> SunSceneObject::getTexture() 
{ 
	return texture; 
}

void SunSceneObject::animateTexture()
{
	texture->nextFrame();
}