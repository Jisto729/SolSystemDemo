#include "Scene.h"

//TODO use classes from dll
#include "Sphere.h"

using namespace ssd;

Scene::Scene()
{
	//TODO change to allow more objects and comunicate with the dll library
	Sphere sphere = Sphere(1);
	std::vector<float> vertices = sphere.getVertices();
	std::vector<float> colors = sphere.getColors();
	std::vector<int> indices = sphere.getIndices();

	SceneObject object1 = SceneObject(vertices, colors, indices);
	objects.push_back(object1);

	verticesBuff = std::make_shared<ge::gl::Buffer>(vertices.size() * sizeof(float), vertices.data());
	colorsBuff = std::make_shared<ge::gl::Buffer>(colors.size() * sizeof(float), colors.data());
	indicesBuff = std::make_shared<ge::gl::Buffer>(indices.size() * sizeof(int), indices.data());
}

//will be changed later, just proof of concept
void Scene::moveObjects()
{
	objects[0].moveObject(0.5f, 0.0f, 0.0f);
	/*std::vector<float> vert = objects[0].getVertices();
	std::cout << "vert:";
	for (int i = 0; i < vert.size(); i++) {
		std::cout << vert[i] << ", ";
	}
	std::vector<float> v(vert.size());
	verticesBuff->getData(v.data());

	std::cout << std::endl << "buff: ";

	for (int i = 0; i < v.size(); i++) {
		std::cout << v[i] << ", ";
	}

	std::cout << std::endl;*/
	updateBuffers();
}

void Scene::updateBuffers()
{
	//TODO change to allow more objects and comunicate with the dll library
	std::vector<float> vertices = objects[0].getVertices();
	//TODO add all buffers, find out if this is the best way to do this
	GLvoid* buffPtr = verticesBuff->map(GL_READ_WRITE);
	memcpy(buffPtr, vertices.data(), verticesBuff->getSize());
	verticesBuff->unmap();
}