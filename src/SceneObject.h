#pragma once 

#include <vector>
#include <glm/glm.hpp>

namespace ssd {
	typedef enum
	{
		normalObject,
		lightObject
	} ObjectType;
	class SceneObject {
	public:
		SceneObject(std::vector<float> inVertices, std::vector<float> inColors, std::vector<int> inIndices, glm::vec3 inCenter, ObjectType inType);
		SceneObject() = default;
		inline std::vector<float> getVertices() { return vertices; };
		inline std::vector<float> getColors() { return colors; };
		inline std::vector<int> getIndices() { return indices; };
		inline glm::vec3 getCenter() { return center; };
		inline glm::mat4 getModelMatrix() { return modelMatrix; };
		inline ObjectType getObjectType() { return type; };

		void moveObject(float dx, float dy, float dz);
		void revolveAroundPoint(glm::vec3 rotCenter, float angle);
		void movePlanet(float rotAngle, float orbitAngle);
		void moveMoon(glm::vec3 rotCenter, float rotAngle, float orbitAngle, float planetOrbitAngle);
		void moveToPosition(glm::vec3 position);
	protected:
		//TODO split to more objects to differentiate between different objects
		std::vector<float> vertices;
		std::vector<float> colors;
		std::vector<int> indices;
		glm::vec3 center;
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		ObjectType type;
	private:
		void updateBuffers();
	};
}