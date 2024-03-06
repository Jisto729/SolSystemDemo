#pragma once 

#include <memory>
#include "SceneObject.h"
#include "SunTexture.h"
#include <geGL/geGL.h>

namespace ssd
{
	class SunSceneObject : public SceneObject
	{
	public:
		SunSceneObject(std::vector<float> inVertices, std::vector<float> inColors, std::vector<int> inIndices, glm::vec3 inCenter, ObjectType inType);
		std::shared_ptr<ssl::SunTexture> getTexture();
		void bindCurrentTexture();
		void unbindCurrentTexture();
		void regenerateTexture(int seed, int temperature);
		void animateTexture();
		int getTexId() { 
			//std::cout << "geting id" << std::endl;
			return textureObject->getId(); }
	private:
		std::shared_ptr<ssl::SunTexture> texture;
		std::shared_ptr<ge::gl::Texture> textureObject;
	};
}