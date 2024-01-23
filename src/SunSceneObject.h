#pragma once 

#include <memory>
#include "SceneObject.h"
#include "SunTexture.h"

namespace ssd
{
	class SunSceneObject : public SceneObject
	{
	public:
		SunSceneObject(std::vector<float> inVertices, std::vector<float> inColors, std::vector<int> inIndices, glm::vec3 inCenter, ObjectType inType);
		std::shared_ptr<ssl::SunTexture> getTexture();
		void animateTexture();
	private:
		std::shared_ptr<ssl::SunTexture> texture;
	};
}