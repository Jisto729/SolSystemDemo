#include "Renderer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Scene.h"
#include <QOpenGLFramebufferObjectFormat>

//TODO remove probably
#include <geUtil/Text.h>
#include <glm/gtx/string_cast.hpp>
#include "Camera.h"
//#include <stb_image.h>

#include "SunTexture.h"

using namespace ssd;

Renderer::Renderer(int width, int height, const qreal pixelRatio, std::shared_ptr<Camera> cameraPtr, std::shared_ptr<Scene> scene, GLuint framebuffer)
{
	gl = std::make_shared<ge::gl::Context>();
	windowWidth = width;
	windowHeight = height;
	camera = cameraPtr;

	planetRenderer = std::make_shared<PlanetRenderer>(scene, camera, gl);
	sunRenderer = std::make_shared<SunRenderer>(scene, camera, gl);
	
	//Creating hdr framebuffer
	baseFramebuffer = std::make_shared<ge::gl::Framebuffer>();
	baseFramebuffer->bind(GL_FRAMEBUFFER);

	baseRenderTex = std::make_shared<ge::gl::Texture>(GL_TEXTURE_2D, GL_RGB16F, 0, windowWidth, windowHeight);
	baseRenderTex->bind(baseRenderTex->getId());
	baseRenderTex->setData2D(NULL, GL_RGB16F, GL_FLOAT, 0, GL_TEXTURE_2D, 0, 0, windowWidth, windowHeight, 0);
	baseRenderTex->texParameteri(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	baseRenderTex->texParameteri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	baseRenderTex->texParameteri(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
	baseRenderTex->texParameteri(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	baseFramebuffer->attachTexture(GL_COLOR_ATTACHMENT0, baseRenderTex, 0, -1);
	baseRenderTex->unbind(baseRenderTex->getId());

	renderBuff = std::make_shared<ge::gl::Renderbuffer>(GL_DEPTH_COMPONENT, windowWidth, windowHeight, 0);
	renderBuff->bind();
	baseFramebuffer->attachRenderbuffer(GL_DEPTH_ATTACHMENT, renderBuff);
	renderBuff->unbind();

	if (!baseFramebuffer->check())
	{
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	}

	gl->glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);

	//TODO maybe move somewhere else
	gl->glEnable(GL_DEPTH_TEST);
	gl->glEnable(GL_STENCIL_TEST);

	bloomRenderer = std::make_shared<BloomRenderer>(scene, camera, gl, glm::vec2(windowWidth, windowHeight));
	gl->glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);

	std::shared_ptr<ge::gl::Shader> combineFragmentShader = std::make_shared<ge::gl::Shader>(GL_FRAGMENT_SHADER, ge::util::loadTextFile(COMBINE_FRAGMENT_SHADER));
	std::shared_ptr<ge::gl::Shader> combineVertexShader = std::make_shared<ge::gl::Shader>(GL_VERTEX_SHADER, ge::util::loadTextFile(COMBINE_VERTEX_SHADER));

	combineShaderProgram = std::make_shared<ge::gl::Program>(combineVertexShader, combineFragmentShader);

	defaultFramebuffer = framebuffer;

	std::vector<float> screenVertices = {
		-1.0f,  1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
	};

	std::vector<float> screenTexCoords = {
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
	};

	screenVerticesBuff = std::make_shared<ge::gl::Buffer>(screenVertices.size() * sizeof(float), screenVertices.data());
	screenTexCoordsBuff = std::make_shared<ge::gl::Buffer>(screenTexCoords.size() * sizeof(float), screenTexCoords.data());

	screenQuadVAO = std::make_shared<ge::gl::VertexArray>();

	screenQuadVAO->addAttrib(screenVerticesBuff, 0, 3, GL_FLOAT);
	screenQuadVAO->addAttrib(screenTexCoordsBuff, 1, 2, GL_FLOAT);
}

void Renderer::updateDimentions(int width, int height)
{
	windowWidth = width;
	windowHeight = height;
}

void Renderer::setDefaultFBO(GLuint framebuffer)
{
	defaultFramebuffer = framebuffer;
}

void Renderer::render(std::shared_ptr<Scene> scene)
{
	//rendering the base image to framebuffer
	baseFramebuffer->bind(GL_FRAMEBUFFER);

	camera->updateMatrix(windowWidth, windowHeight);
	gl->glViewport(0, 0, windowWidth, windowHeight);
	gl->glClearColor(0.0, 0.0, 0.0, 1.0);
	gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	planetRenderer->render();
	sunRenderer->render();

	gl->glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);

	bloomRenderer->render(baseRenderTex);
	bloomRenderTex = bloomRenderer->getTexture();

	gl->glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);

	//rendering final

	gl->glViewport(0, 0, windowWidth, windowHeight);
	gl->glClearColor(0.0, 1.0, 0.0, 1.0);
	gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	baseRenderTex->bind(baseRenderTex->getId());
	combineShaderProgram->set1i("scene", baseRenderTex->getId());
	bloomRenderTex->bind(bloomRenderTex->getId());
	combineShaderProgram->set1i("bloomBlur", bloomRenderTex->getId());

	combineShaderProgram->validate();
	combineShaderProgram->use();

	screenQuadVAO->bind();
	gl->glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	screenQuadVAO->unbind();
}