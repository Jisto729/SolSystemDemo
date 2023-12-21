#pragma once 

#include <vector>
#include <glm/glm.hpp>

namespace ssd {
	class SceneObject {
	public:
		SceneObject(std::vector<float> inVertices, std::vector<float> inColors, std::vector<int> inIndices);
		inline std::vector<float> getVertices() { return vertices; };
		inline std::vector<float> getColors() { return colors; };
		inline std::vector<int> getIndices() { return indices; };
		inline glm::mat4 getModelMatrix() { return modelMatrix; };

		void moveObject(float dx, float dy, float dz);
	private:
		std::vector<float> vertices;
		std::vector<float> colors;
		std::vector<int> indices;
		glm::mat4 modelMatrix = glm::mat4(1.0f);

		void updateBuffers();
	};
}