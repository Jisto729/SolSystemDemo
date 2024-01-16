#include "Scene.h"
#include <algorithm>
#include <glm/gtx/string_cast.hpp>

#include "Sphere.h"
#include "CubeSphere.h"

using namespace ssd;

Scene::Scene()
{
	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> colors;
	std::vector<int> indices;
	//should probably be unsigned int, but can't seem to make that work
	std::vector<float> modelIDs;

	std::vector<ssl::CubeSphere> cubeLights;
	ssl::CubeSphere cubeSun = ssl::CubeSphere(1.0, 4);
	cubeSun.setColor(glm::vec3(1.0, 0.5, 1.0));
	cubeLights.push_back(cubeSun); //sun

	//TODO change to some function, so that it is not here twice
	int modelID = 0;
	for (ssl::CubeSphere light : cubeLights)
	{
		std::vector<float> objVertices = light.getVertices();
		std::vector<float> objColors = light.getColors();
		std::vector<int> objIndices = light.getIndices();

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

		objects.push_back(SceneObject(objVertices, objColors, objIndices, light.getCenter(), lightObject));
		vertices.insert(vertices.end(), objVertices.begin(), objVertices.end());
		colors.insert(colors.end(), objColors.begin(), objColors.end());
		indices.insert(indices.end(), objIndices.begin(), objIndices.end());
		modelID++;
	}

	lightVerticesBuff = std::make_shared<ge::gl::Buffer>(vertices.size() * sizeof(float), vertices.data());
	lightColorsBuff = std::make_shared<ge::gl::Buffer>(colors.size() * sizeof(float), colors.data());
	lightIndicesBuff = std::make_shared<ge::gl::Buffer>(indices.size() * sizeof(int), indices.data());
	lightModelIDsBuff = std::make_shared<ge::gl::Buffer>(modelIDs.size() * sizeof(float), modelIDs.data());

	vertices.clear();
	colors.clear();
	indices.clear();
	modelIDs.clear();

	std::vector<ssl::Sphere> spheres;

	ssl::Sphere planet = ssl::Sphere(1, 5.0f, 0.0f, 5.0f);
	planet.setColor(glm::vec3(0.2, 0.2, 0.9));
	ssl::Sphere moon = ssl::Sphere(1, 5.0f, 0.0f, 7.5f);
	moon.setColor(glm::vec3(0.4, 0.4, 0.4));
	spheres.push_back(planet); //planet
	spheres.push_back(moon); //moon

	//change to object or something like that
	modelID = 0;
	for (ssl::Sphere sphere : spheres)
	{
		std::vector<float> objVertices = sphere.getVertices();
		std::vector<float> objNormals = sphere.getNormals();
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

		objects.push_back(SceneObject(objVertices, objColors, objIndices, sphere.getCenter(), normalObject));
		vertices.insert(vertices.end(), objVertices.begin(), objVertices.end());
		normals.insert(normals.end(), objNormals.begin(), objNormals.end());
		colors.insert(colors.end(), objColors.begin(), objColors.end());
		indices.insert(indices.end(), objIndices.begin(), objIndices.end());
		modelID++;
	}

	verticesBuff = std::make_shared<ge::gl::Buffer>(vertices.size() * sizeof(float), vertices.data());
	normalsBuff = std::make_shared<ge::gl::Buffer>(normals.size() * sizeof(float), normals.data());
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
		float planetOrbit = 1.0f;
		//simulating sun, planet and a moon, will be later done according to some properties
		if (i == 0)
		//sun
		{
			obj.revolveAroundPoint(systemCenter, glm::radians(1.0f));
		} else if (i == 1)
		//planet
		{
			obj.movePlanet(glm::radians(1.0f), glm::radians(planetOrbit));
		} else if (i == 2)
		//moon
		{
			obj.moveMoon(objects[1].getCenter(), glm::radians(10.0f), glm::radians(-5.0f), glm::radians(planetOrbit));
		}
		i++;
	}
}

std::vector<glm::vec3> Scene::getLightPositions()
{
	std::vector<glm::vec3> lights;
	for (SceneObject obj : objects)
	{
		if (obj.getObjectType() == lightObject)
		{
			lights.push_back(obj.getCenter());
		}
	}
	return lights;
}

std::vector<glm::vec3> Scene::getLightColors()
{
	std::vector<glm::vec3> colors;
	for (SceneObject obj : objects)
	{
		if (obj.getObjectType() == lightObject)
		{
			//TODO maybe average all colors, currently the first color only is taken
			std::vector<float> objColors = obj.getColors();
			colors.push_back(glm::vec3(objColors[0], objColors[1], objColors[2]));
		}
	}
	return colors;
}

int Scene::getLightCount()
{
	int lights = 0;
	for (SceneObject obj : objects)
	{
		if (obj.getObjectType() == lightObject)
		{
			lights++;
		}
	}
	return lights;
}

std::vector<glm::mat4> Scene::getModelMatrices(ObjectType type)
{
	std::vector<glm::mat4> matrices;
	for (SceneObject obj : objects)
	{
		if (obj.getObjectType() == type)
		{
			glm::mat4 matrix = obj.getModelMatrix();
			matrices.push_back(matrix);
		}
	}
	return matrices;
}

void Scene::updateModelMatrices(std::shared_ptr<ge::gl::Buffer> SSBO, ObjectType type)
{
	std::vector<glm::mat4> matrices = getModelMatrices(type);
	GLvoid* buffPtr = SSBO->map(GL_READ_WRITE);
	memcpy(buffPtr, matrices.data(), SSBO->getSize());
	SSBO->unmap();
}