#pragma once 

#include <vector>

namespace ssd {
	class SceneObject {
	public:
		SceneObject(std::vector<float> inVertices, std::vector<float> inColors, std::vector<int> inIndices);
		inline std::vector<float> getVertices() { return vertices; };
		inline std::vector<float> getColors() { return colors; };
		inline std::vector<int> getIndices() { return indices; };
	private:
		std::vector<float> vertices;
		std::vector<float> colors;
		std::vector<int> indices;
	};
}