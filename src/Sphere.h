#pragma once

#include <vector>
#include <glm/glm.hpp>

namespace ssd {
	class Sphere {
	public:
		Sphere(int size, float posX, float posY, float posZ);
		void setColor(glm::vec3 color);
		inline std::vector<float> getVertices() { return GLvertices; };
		inline std::vector<float> getColors() { return colors; };
		inline std::vector<int> getIndices() { return GLindices; };

	private:
		std::vector<float> GLvertices;
        std::vector<glm::vec3> vertices;
		std::vector<float> colors;
		std::vector<int> GLindices;
		std::vector<glm::vec3> indices;

		void Sphere::icosahedron(float posX, float posY, float posZ);
		void Sphere::setUniqueColors();
		std::vector<float> Sphere::convertToGLData(std::vector<glm::vec3> data);
	};
}