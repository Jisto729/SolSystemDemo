#include "Scene.h"

//TODO use classes from dll
#include "Sphere.h"

using namespace ssd;

Scene::Scene() {
	Sphere sphere = Sphere(1);
	SceneObject object1 = SceneObject(sphere.getVertices(), sphere.getColors(), sphere.getIndices());
	objects.push_back(object1);
}