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
		void updateBuffers();
		inline std::vector<SceneObject> getObjects() { return objects; }

		inline std::shared_ptr<ge::gl::Buffer> getVerticesBuff() { return verticesBuff; };
		inline std::shared_ptr<ge::gl::Buffer> getColorsBuff() { return colorsBuff; };
		inline std::shared_ptr<ge::gl::Buffer> getIndicesBuff() { return indicesBuff; };
	private:
		std::vector<SceneObject> objects;

		std::shared_ptr<ge::gl::Buffer> verticesBuff;
		std::shared_ptr<ge::gl::Buffer> colorsBuff;
		std::shared_ptr<ge::gl::Buffer> indicesBuff;
	};
}