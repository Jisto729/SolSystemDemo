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


		inline std::shared_ptr<ge::gl::Buffer> getVerticesBuff(ObjectType type) 
		{
			return (type == normalObject) ? verticesBuff : lightVerticesBuff;
		};
		inline std::shared_ptr<ge::gl::Buffer> getNormalsBuff() { return normalsBuff; };
		inline std::shared_ptr<ge::gl::Buffer> getColorsBuff(ObjectType type) 
		{
			return (type == normalObject) ? colorsBuff : lightColorsBuff;
		};
		inline std::shared_ptr<ge::gl::Buffer> getIndicesBuff(ObjectType type)
		{ 
			return (type == normalObject) ? indicesBuff : lightIndicesBuff; 
		};
		inline std::shared_ptr<ge::gl::Buffer> getModelIDsBuff(ObjectType type) 
		{ 
			return (type == normalObject) ? modelIDsBuff : lightModelIDsBuff; ;
		};
		std::vector<glm::mat4> getModelMatrices(ObjectType type);
		std::vector<glm::vec3> getLightPositions();
		std::vector<glm::vec3> getLightColors();
		int getLightCount();
		void updateModelMatrices(std::shared_ptr<ge::gl::Buffer> SSBO, ObjectType type);
	
	private:
		std::vector<SceneObject> objects;

		//TODO probably add new class to hold all these buffers?
		std::shared_ptr<ge::gl::Buffer> verticesBuff;
		std::shared_ptr<ge::gl::Buffer> normalsBuff;
		std::shared_ptr<ge::gl::Buffer> colorsBuff;
		std::shared_ptr<ge::gl::Buffer> indicesBuff;
		std::shared_ptr<ge::gl::Buffer> modelIDsBuff;

		std::shared_ptr<ge::gl::Buffer> lightVerticesBuff;
		std::shared_ptr<ge::gl::Buffer> lightColorsBuff;
		std::shared_ptr<ge::gl::Buffer> lightIndicesBuff;
		std::shared_ptr<ge::gl::Buffer> lightModelIDsBuff;
	};
}