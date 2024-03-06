#include "Scene.h"
#include "SunSceneObject.h"
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
	std::vector<float> texCoords;
	//should probably be unsigned int, but can't seem to make that work
	std::vector<float> modelIDs;

	std::vector<ssl::CubeSphere> cubeLights;
	ssl::CubeSphere cubeSun = ssl::CubeSphere(3.0, 4);
	cubeSun.setColor(glm::vec3(1.0, 1.0, 1.0));
	cubeLights.push_back(cubeSun); //sun

	//TODO change to some function, so that it is not here twice
	int modelID = 0;
	for (ssl::CubeSphere light : cubeLights)
	{
		std::vector<float> objVertices = light.getVertices();
		std::vector<float> objColors = light.getColors();
		std::vector<int> objIndices = light.getIndices();
		std::vector<float> objTexCoords = light.getTexCoords();

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

		//TODO temporary
		lightObjects.push_back(SunSceneObject(objVertices, objColors, objIndices, light.getCenter(), lightObject));
		
		objects.push_back(SceneObject(objVertices, objColors, objIndices, light.getCenter(), lightObject));
		vertices.insert(vertices.end(), objVertices.begin(), objVertices.end());
		colors.insert(colors.end(), objColors.begin(), objColors.end());
		indices.insert(indices.end(), objIndices.begin(), objIndices.end());
		texCoords.insert(texCoords.end(), objTexCoords.begin(), objTexCoords.end());
		modelID++;
	}

	lightVerticesBuff = std::make_shared<ge::gl::Buffer>(vertices.size() * sizeof(float), vertices.data());
	lightColorsBuff = std::make_shared<ge::gl::Buffer>(colors.size() * sizeof(float), colors.data());
	lightIndicesBuff = std::make_shared<ge::gl::Buffer>(indices.size() * sizeof(int), indices.data());
	lightTexBuff = std::make_shared<ge::gl::Buffer>(texCoords.size() * sizeof(float), texCoords.data());
	lightModelIDsBuff = std::make_shared<ge::gl::Buffer>(modelIDs.size() * sizeof(float), modelIDs.data());

	vertices.clear();
	colors.clear();
	indices.clear();
	modelIDs.clear();

	for (float coord : texCoords)
	{
		//std::cout << coord << std::endl;
	}

	//std::vector<ssl::Sphere> spheres;

	//ssl::Sphere planet = ssl::Sphere(1, 5.0f, 0.0f, 5.0f);
	//planet.setColor(glm::vec3(0.2, 0.2, 0.9));
	//ssl::Sphere moon = ssl::Sphere(1, 5.0f, 0.0f, 7.5f);
	//moon.setColor(glm::vec3(0.4, 0.4, 0.4));
	//spheres.push_back(planet); //planet
	//spheres.push_back(moon); //moon

	std::vector<ssl::CubeSphere> spheres;

	ssl::CubeSphere planet1 = ssl::CubeSphere(0.7, 8.0f, 0.0f, 8.0f, 4);
	planet1.setColor(glm::vec3(0.2, 0.2, 0.9));
	ssl::CubeSphere planet2 = ssl::CubeSphere(1, 16.0f, 0.0f, 0.0f, 4);
	planet2.setColor(glm::vec3(0.7, 0.2, 0.2));
	ssl::CubeSphere planet3 = ssl::CubeSphere(2, 25.0f, 0.0f, 2.0f, 4);
	planet3.setColor(glm::vec3(0.3, 0.3, 0.9));
	spheres.push_back(planet1); //planet
	spheres.push_back(planet2); //planet
	spheres.push_back(planet3); //planet
	ssl::CubeSphere moon1 = ssl::CubeSphere(0.25, 8.0f, 0.0f, 7.0f, 4);
	moon1.setColor(glm::vec3(0.7, 0.7, 0.7));
	ssl::CubeSphere moon2 = ssl::CubeSphere(0.38, 16.0f, 0.0f, 2.5f, 4);
	moon2.setColor(glm::vec3(0.4, 0.4, 0.4));
	ssl::CubeSphere moon3 = ssl::CubeSphere(0.3, 16.3f, 0.0f, -1.4f, 4);
	moon3.setColor(glm::vec3(0.4, 0.5, 0.5));
	ssl::CubeSphere moon4 = ssl::CubeSphere(1.1, 26.0f, 0.0f, 5.2f, 4);
	moon4.setColor(glm::vec3(0.2, 0.2, 0.2));
	spheres.push_back(moon1); //moon
	spheres.push_back(moon2); //moon
	spheres.push_back(moon3); //moon
	spheres.push_back(moon4); //moon

	//change to object or something like that
	modelID = 0;
	//for (ssl::Sphere sphere : spheres)
	for (ssl::CubeSphere sphere : spheres)
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
		float planetOrbit1 = 1.0f;
		float planetOrbit2 = 0.4f;
		float planetOrbit3 = 0.7f;
		//simulating sun, planet and a moon, will be later done according to some properties
		switch (i)
		{
		case 0:
			obj.revolveAroundPoint(systemCenter, glm::radians(0.3f));
			break;
		case 1:
			obj.movePlanet(glm::radians(1.0f), glm::radians(planetOrbit1));
			break;
		case 2:
			obj.movePlanet(glm::radians(0.5f), glm::radians(planetOrbit2));
			break;
		case 3:
			obj.movePlanet(glm::radians(-1.0f), glm::radians(planetOrbit3));
			break;
		case 4:
			obj.moveMoon(objects[1].getCenter(), glm::radians(6.0f), glm::radians(-5.0f), glm::radians(planetOrbit1));
			break;
		case 5:
			obj.moveMoon(objects[2].getCenter(), glm::radians(20.0f), glm::radians(5.0f), glm::radians(planetOrbit2));
			break;
		case 6:
			obj.moveMoon(objects[2].getCenter(), glm::radians(10.0f), glm::radians(3.0f), glm::radians(planetOrbit2));
			break;
		case 7:
			obj.moveMoon(objects[3].getCenter(), glm::radians(10.0f), glm::radians(1.0f), glm::radians(planetOrbit3));
			break;
		}
		//if (i == 0)
		////sun
		//{
		//	obj.revolveAroundPoint(systemCenter, glm::radians(0.3f));
		//} else if (i == 1)
		////planets
		//{
		//	obj.movePlanet(glm::radians(1.0f), glm::radians(planetOrbit));
		//} else if (i == 2)
		////moon
		//{
		//	obj.moveMoon(objects[1].getCenter(), glm::radians(10.0f), glm::radians(-5.0f), glm::radians(planetOrbit));
		//}
		i++;
	}
}

void Scene::animateSurfaces()
{
	for (SunSceneObject& obj : lightObjects)
	{
		obj.animateTexture();
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
	//std::cout << "updating matrices" << std::endl;
	std::vector<glm::mat4> matrices = getModelMatrices(type);
	//std::cout << "updating matrices2" << std::endl;

	GLvoid* buffPtr = SSBO->map(GL_READ_WRITE);
	//std::cout << SSBO->getSize();
	//std::cout << "updating matrices3" << std::endl;
	memcpy(buffPtr, matrices.data(), SSBO->getSize());
	//std::cout << "updating matrice4" << std::endl;

	SSBO->unmap();
}

void Scene::update(GenAttributes attribs)
{
	for (SunSceneObject& obj : lightObjects)
	{
		obj.regenerateTexture(attribs.seed, attribs.sunTemperature);
	}
}
