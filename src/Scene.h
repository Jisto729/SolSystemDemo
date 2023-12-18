#pragma once 

#include <vector>
#include "SceneObject.h"

namespace ssd {
	class Scene {
	public:
		Scene();
		inline std::vector<SceneObject> getObjects() { return objects; }
	private:
		std::vector<SceneObject> objects;
	};
}