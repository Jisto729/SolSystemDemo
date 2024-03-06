#pragma once 

#include <memory>
#include <vector>
#include <geGL/geGL.h>
//#include <QtGui/QOpenGLContext>
#include <QOpenGLFramebufferObject>
#include "Camera.h"
#include "Scene.h"
#include "PlanetRenderer.h"
#include "SunRenderer.h"
#include "BloomRenderer.h"

namespace ssd
{
	class Renderer
	{
	public:

		Renderer(int width, int height, const qreal pixelRatio, std::shared_ptr<Camera> cameraPtr, std::shared_ptr<Scene> scene, GLuint framebuffer);

		void updateDimentions(int width, int height);
		void setDefaultFBO(GLuint framebuffer);
		void render(std::shared_ptr<Scene> scene);
		//TODO add window resize function handler
	private:

		int windowWidth;
		int windowHeight;

		std::shared_ptr<ge::gl::Context> gl;

		std::shared_ptr<ge::gl::Program> combineShaderProgram;
		std::shared_ptr<ge::gl::Framebuffer> baseFramebuffer;
		std::shared_ptr<ge::gl::Renderbuffer> renderBuff;
		
		//rendering texture to screen
		std::shared_ptr<ge::gl::Buffer> screenVerticesBuff;
		std::shared_ptr<ge::gl::Buffer> screenTexCoordsBuff;
		std::shared_ptr<ge::gl::VertexArray> screenQuadVAO;

		GLuint defaultFramebuffer;

		//textures, to render into
		std::shared_ptr<ge::gl::Texture> baseRenderTex;
		std::shared_ptr<ge::gl::Texture> bloomRenderTex;
		//renderers
		std::shared_ptr<PlanetRenderer> planetRenderer;
		std::shared_ptr<SunRenderer> sunRenderer;
		std::shared_ptr<BloomRenderer> bloomRenderer;

		std::shared_ptr<Camera> camera;
	};
}