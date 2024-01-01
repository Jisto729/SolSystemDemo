#include "Scene.h"
#include <algorithm>
#include <glm/gtx/string_cast.hpp>

#include "Sphere.h"

using namespace ssd;

Scene::Scene()
{
	std::vector<ssl::Sphere> spheres;
	spheres.push_back(ssl::Sphere(1, 0.0f, 0.0f, 0.0f)); //sun
	spheres.push_back(ssl::Sphere(1, 5.0f, 0.0f, 5.0f)); //planet
	spheres.push_back(ssl:: Sphere(1, 5.0f, 0.0f, 7.5f)); //moon

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
	glm::vec3 systemCenter = glm::vec3(0.0f, 0.0f, 0.0f);
	for (SceneObject& obj : objects)
	{
		float planetOrbit = -5.0f;
		//simulating sun, planet and a moon, will be later done according to some properties
		if (i == 0)
		//sun
		{
			obj.revolveAroundPoint(systemCenter, glm::radians(1.0f));
		} else if (i == 1)
		//planet
		{
			obj.movePlanet(glm::radians(9.0f), glm::radians(planetOrbit));
		} else if (i == 2)
		//moon
		{
			obj.moveMoon(objects[1].getCenter(), glm::radians(3.0f), glm::radians(-10.0f), glm::radians(planetOrbit));
		}
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