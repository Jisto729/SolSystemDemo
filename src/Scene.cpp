#include "Scene.h"
#include <algorithm>
#include <glm/gtx/string_cast.hpp>


//TODO use classes from dll
#include "Sphere.h"

using namespace ssd;

Scene::Scene()
{
	//TODO comunicate with the dll library
	std::vector<ssl::Sphere> spheres;
	spheres.push_back(ssl::Sphere(1, 0.0f, 0.0f, 0.0f));
	spheres.push_back(ssl::Sphere(1, 2.0f, 2.0f, 2.0f));
	spheres.push_back(ssl:: Sphere(1, 5.0f, 3.0f, 3.0f));
	spheres.push_back(ssl::Sphere(1, 1.0f, 1.0f, 1.0f));
	spheres.push_back(ssl::Sphere(1, -1.0f, -1.0f, 1.0f));
	spheres.push_back(ssl::Sphere(1, -1.0f, 1.0f, 1.0f));


	std::vector<float> vertices;
	std::vector<float> colors;
	std::vector<int> indices;
	//should probably be unsigned int, but can't seem to make that work
	std::vector<float> modelIDs;

	//change to object or something like that
	int modelID = 0;
	for (ssl::Sphere sphere : spheres)
	{
		std::vector<float> objVertices = sphere.getVertices();
		std::vector<float> objColors = sphere.getColors();
		std::vector<int> objIndices = sphere.getIndices();

		//for each vertex a modelID is added also
		for (int i = 0; i < objVertices.size(); i += 3)
		{
			modelIDs.push_back(modelID);
		}

		//recalculating indices if the scene is composed of more objects
		int indexOffset = vertices.size() / 3;
		for (int& index : objIndices)
		{
			index += indexOffset;
		}

		objects.push_back(SceneObject(objVertices, objColors, objIndices, sphere.getCenter()));
		vertices.insert(vertices.end(), objVertices.begin(), objVertices.end());
		colors.insert(colors.end(), objColors.begin(), objColors.end());
		indices.insert(indices.end(), objIndices.begin(), objIndices.end());
		modelID++;
	}

	verticesBuff = std::make_shared<ge::gl::Buffer>(vertices.size() * sizeof(float), vertices.data());
	colorsBuff = std::make_shared<ge::gl::Buffer>(colors.size() * sizeof(float), colors.data());
	indicesBuff = std::make_shared<ge::gl::Buffer>(indices.size() * sizeof(int), indices.data());
	modelIDsBuff = std::make_shared<ge::gl::Buffer>(modelIDs.size() * sizeof(float), modelIDs.data());
}

//will be changed later, just proof of concept
void Scene::moveObjects()
{
	int i = 0;
	for (SceneObject& obj : objects)
	{
		obj.revolveAroundPoint(obj.getCenter(), glm::radians(1.0f));
		i++;
	}
}

std::vector<glm::mat4> Scene::getModelMatrices()
{
	std::vector<glm::mat4> matrices;
	for (SceneObject obj : objects)
	{
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