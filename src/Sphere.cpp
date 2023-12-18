#include "Sphere.h"
#include <iostream>

using namespace ssd;

Sphere::Sphere(int size) {
	Sphere::icosahedron();
    Sphere::setUniqueColors();
}

void Sphere::icosahedron() {

    //from https://danielsieger.com/blog/2021/01/03/generating-platonic-solids.html
	float phi = (1.0f + sqrt(5.0f)) * 0.5f; // golden ratio
	float a = 1.0f;
	float b = 1.0f / phi;

	// adding vertices
    vertices.push_back(glm::vec3(0, b, -a));
    vertices.push_back(glm::vec3(b, a, 0));
    vertices.push_back(glm::vec3(-b, a, 0));
    vertices.push_back(glm::vec3(0, b, a));
    vertices.push_back(glm::vec3(0, -b, a));
    vertices.push_back(glm::vec3(-a, 0, b));
    vertices.push_back(glm::vec3(0, -b, -a));
    vertices.push_back(glm::vec3(a, 0, -b));
    vertices.push_back(glm::vec3(a, 0, b));
    vertices.push_back(glm::vec3(-a, 0, -b));
    vertices.push_back(glm::vec3(b, -a, 0));
    vertices.push_back(glm::vec3(-b, -a, 0));

    // adding triangles
    indices.push_back(glm::vec3(2, 1, 0));
    indices.push_back(glm::vec3(1, 2, 3));
    indices.push_back(glm::vec3(5, 4, 3));
    indices.push_back(glm::vec3(4, 8, 3));
    indices.push_back(glm::vec3(7, 6, 0));
    indices.push_back(glm::vec3(6, 9, 0));
    indices.push_back(glm::vec3(11, 10, 4));
    indices.push_back(glm::vec3(10, 11, 6));
    indices.push_back(glm::vec3(9, 5, 2));
    indices.push_back(glm::vec3(5, 9, 11));
    indices.push_back(glm::vec3(8, 7, 1));
    indices.push_back(glm::vec3(7, 8, 10));
    indices.push_back(glm::vec3(2, 5, 3));
    indices.push_back(glm::vec3(8, 1, 3));
    indices.push_back(glm::vec3(9, 2, 0));
    indices.push_back(glm::vec3(1, 7, 0));
    indices.push_back(glm::vec3(11, 9, 6));
    indices.push_back(glm::vec3(7, 10, 6));
    indices.push_back(glm::vec3(5, 11, 4));
    indices.push_back(glm::vec3(10, 8, 4));

    GLvertices = Sphere::convertToGLData(vertices);
    std::vector<float> flIndices = Sphere::convertToGLData(indices);
    GLindices = std::vector<int>(flIndices.begin(), flIndices.end());
}

void Sphere::setColor(glm::vec3 color) {
    for (glm::vec3 vertex : vertices) {
        colors.push_back(color[0]);
        colors.push_back(color[1]);
        colors.push_back(color[2]);
    }
}

void Sphere::setUniqueColors() {
    float color = 0;
    float diff = 1.0/vertices.size();
    for (glm::vec3 vertex : vertices) {
        colors.push_back(color);
        colors.push_back(color);
        colors.push_back(color);
        color += diff;
    }
}

std::vector<float> Sphere::convertToGLData(std::vector<glm::vec3> data) {
    std::vector<float> returnData;
    for (glm::vec3 item : data) {
        returnData.push_back(item[0]);
        returnData.push_back(item[1]);
        returnData.push_back(item[2]);
    }
    return returnData;
}