#pragma once

#include "Scene.h"
#include "Camera.h"
#include <geGL/geGL.h>

namespace ssd
{
	class PlanetRenderer
	{
	public:
		PlanetRenderer(std::shared_ptr<Scene> scene, std::shared_ptr<Camera> camera, std::shared_ptr<ge::gl::Context> gl);
		void render();
	private:
		std::shared_ptr<Scene> scene;
		std::shared_ptr<Camera> camera;
		std::shared_ptr<ge::gl::Context> gl;
		std::shared_ptr<ge::gl::Program> shaderProgram;
		std::shared_ptr<ge::gl::VertexArray> VAO;
		std::shared_ptr<ge::gl::Buffer> SSBO;
	};
}