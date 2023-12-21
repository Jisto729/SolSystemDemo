#include "Scene.h"
#include <algorithm>

//TODO use classes from dll
#include "Sphere.h"

using namespace ssd;

Scene::Scene()
{
	//TODO change to allow more objects and comunicate with the dll library
	std::vector<Sphere> spheres;
	spheres.push_back(Sphere(1, 0.0f, 0.0f, 0.0f));
	spheres.push_back(Sphere(1, 3.0f, 3.0f, 3.0f));
	spheres.push_back(Sphere(1, 1.0f, 1.0f, 1.0f));

	std::vector<float> vertices;
	std::vector<float> colors;
	std::vector<int> indices;

	//change to object or something like that
	for (Sphere sphere : spheres) {
		std::vector<float> objVertices = sphere.getVertices();
		std::vector<float> objColors = sphere.getColors();
		std::vector<int> objIndices = sphere.getIndices();

		//recalculating indices if the scene is composed of more objects
		int indexOffset = vertices.size() / 3;
		std::cout << indexOffset;
		for (int& index : objIndices) {
			index += indexOffset;
		}

		objects.push_back(SceneObject(objVertices, objColors, objIndices));
		vertices.insert(vertices.end(), objVertices.begin(), objVertices.end());
		colors.insert(colors.end(), objColors.begin(), objColors.end());
		indices.insert(indices.end(), objIndices.begin(), objIndices.end());
		for (float pos : objVertices) {

		}
	}

	verticesBuff = std::make_shared<ge::gl::Buffer>(vertices.size() * sizeof(float), vertices.data());
	colorsBuff = std::make_shared<ge::gl::Buffer>(colors.size() * sizeof(float), colors.data());
	indicesBuff = std::make_shared<ge::gl::Buffer>(indices.size() * sizeof(int), indices.data());
}

//will be changed later, just proof of concept
void Scene::moveObjects()
{
	for (SceneObject& obj : objects) {
		obj.moveObject(0.05f, 0.0f, 0.0f);
	}
	//updateBuffers();
}

void Scene::updateBuffers()
{
	//TODO change to allow more objects and comunicate with the dll library
//	std::vector<float> vertices;
//
//	for (SceneObject obj : objects) {
//		std::vector<float> objVertices = obj.getVertices();
//		vertices.insert(vertices.end(), objVertices.begin(), objVertices.end());
//	}
//
//	//TODO add all buffers, find out if this is the best way to do this
//	GLvoid* buffPtr = verticesBuff->map(GL_READ_WRITE);
//	memcpy(buffPtr, vertices.data(), verticesBuff->getSize());
//	verticesBuff->unmap();
}

std::vector<glm::mat4> Scene::getModelMatrices()
{
	std::vector<glm::mat4> matrices;
	for (SceneObject obj : objects) {
		glm::mat4 matrix = obj.getModelMatrix();
		matrices.push_back(matrix);
	}
	return matrices;
}

void Scene::updateModelMatrices(std::shared_ptr<ge::gl::Buffer> SSBO)
{
	std::vector<glm::mat4> matrices = getModelMatrices();
	GLvoid* buffPtr = SSBO->map(GL_READ_WRITE);
	memcpy(buffPtr, matrices.data(), SSBO->getSize());
	SSBO->unmap();
}