#pragma once 

#include <vector>
#include <geGL/geGL.h>
#include "SceneObject.h"


namespace ssd
{
	class Scene
	{
	public:
		Scene();
		void moveObjects();
		inline std::vector<SceneObject> getObjects() { return objects; }

		inline std::shared_ptr<ge::gl::Buffer> getVerticesBuff() { return verticesBuff; };
		inline std::shared_ptr<ge::gl::Buffer> getColorsBuff() { return colorsBuff; };
		inline std::shared_ptr<ge::gl::Buffer> getIndicesBuff() { return indicesBuff; };
		std::vector<glm::mat4> getModelMatrices();
		void updateModelMatrices(std::shared_ptr<ge::gl::Buffer> SSBO);
	private:
		std::vector<SceneObject> objects;

		std::shared_ptr<ge::gl::Buffer> verticesBuff;
		std::shared_ptr<ge::gl::Buffer> colorsBuff;
		std::shared_ptr<ge::gl::Buffer> indicesBuff;
	};
}