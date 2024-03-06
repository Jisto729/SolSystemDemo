#pragma once

#include "Scene.h"
#include "Camera.h"
#include <geGL/geGL.h>

namespace ssd
{
	struct bloomMip
	{
		glm::vec2 size;
		std::shared_ptr<ge::gl::Texture> texture;
	};

	class BloomRenderer
	{
	public:
		BloomRenderer(std::shared_ptr<Scene> scene, std::shared_ptr<Camera> camera, std::shared_ptr<ge::gl::Context> gl, glm::vec2 windowSize);
		void render(std::shared_ptr<ge::gl::Texture> baseTexture);
		std::shared_ptr<ge::gl::Texture> getTexture()
		{
			return bloomChain[0].texture;
		}
	private:


		std::vector<bloomMip> bloomChain;

		std::shared_ptr<Scene> scene;
		std::shared_ptr<Camera> camera;
		std::shared_ptr<ge::gl::Context> gl;
		std::shared_ptr<ge::gl::Program> downsampleShaderProgram;
		std::shared_ptr<ge::gl::Program> upsampleShaderProgram;
		//std::shared_ptr<ge::gl::Program> combineShaderProgram;
		std::shared_ptr<ge::gl::Framebuffer> bloomFramebuffer;
		std::shared_ptr<ge::gl::VertexArray> VAO;
		std::shared_ptr<ge::gl::Buffer> SSBO;

		glm::vec2 windowSize;

		//rendering texture to screen
		std::shared_ptr<ge::gl::Buffer> screenVerticesBuff;
		std::shared_ptr<ge::gl::Buffer> screenTexCoordsBuff;
		std::shared_ptr<ge::gl::VertexArray> screenQuadVAO;
	};
}