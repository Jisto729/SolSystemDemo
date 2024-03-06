#include "BloomRenderer.h"
#include <geUtil/Text.h>
#include <glm/gtc/type_ptr.hpp>

using namespace ssd;

BloomRenderer::BloomRenderer(std::shared_ptr<Scene> scene, std::shared_ptr<Camera> camera, std::shared_ptr<ge::gl::Context> gl, glm::vec2 windowSize)
	: scene(scene),
	camera(camera),
	gl(gl),
	windowSize(windowSize)
{
	bloomFramebuffer = std::make_shared<ge::gl::Framebuffer>();
	bloomFramebuffer->bind(GL_FRAMEBUFFER);

	int bloomChainLength = 4;
	glm::vec2 mipSize = glm::vec2(windowSize.x, windowSize.y);

	for (int i = 0; i < bloomChainLength; i++)
	{
		bloomMip mip;
		//size cannot be more than 1, 1
		if (mipSize.x < 1 || mipSize.y < 1)
			break;
		mipSize /= 2;
		mip.size = mipSize;

		mip.texture = std::make_shared<ge::gl::Texture>(GL_TEXTURE_2D, GL_R11F_G11F_B10F, 0, mip.size.x, mip.size.y);
		mip.texture->bind(mip.texture->getId());
		mip.texture->setData2D(NULL, GL_R11F_G11F_B10F, GL_FLOAT, 0, GL_TEXTURE_2D, 0, 0, mip.size.x, mip.size.y, 0);
		mip.texture->texParameteri(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		mip.texture->texParameteri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		mip.texture->texParameteri(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
		mip.texture->texParameteri(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		std::cout << "Created bloom mip " << mipSize.x << 'x' << mipSize.y << std::endl;
		bloomChain.emplace_back(mip);
		mip.texture->bind(mip.texture->getId());
	}

	bloomFramebuffer->attachTexture(GL_COLOR_ATTACHMENT0, bloomChain[0].texture, 0, -1);
	unsigned int bloomAttachments[1] = { GL_COLOR_ATTACHMENT0 };
	bloomFramebuffer->drawBuffers(1, bloomAttachments);

	if (!bloomFramebuffer->check())
	{
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	}

	std::cout << "creating bloom shaders" << std::endl;

	std::shared_ptr<ge::gl::Shader> upsampleFragmentShader = std::make_shared<ge::gl::Shader>(GL_FRAGMENT_SHADER, ge::util::loadTextFile(BLOOM_UP_FRAGMENT_SHADER));
	std::shared_ptr<ge::gl::Shader> downsampleFragmentShader = std::make_shared<ge::gl::Shader>(GL_FRAGMENT_SHADER, ge::util::loadTextFile(BLOOM_DOWN_FRAGMENT_SHADER));
	std::shared_ptr<ge::gl::Shader> screenQuadVertexShader = std::make_shared<ge::gl::Shader>(GL_VERTEX_SHADER, ge::util::loadTextFile(SCREEN_QUAD_VERTEX_SHADER));

	upsampleShaderProgram = std::make_shared<ge::gl::Program>(screenQuadVertexShader, upsampleFragmentShader);
	downsampleShaderProgram = std::make_shared<ge::gl::Program>(screenQuadVertexShader, downsampleFragmentShader);

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

void BloomRenderer::render(std::shared_ptr<ge::gl::Texture> baseTexture)
{
	//downsampling

	bloomFramebuffer->bind(GL_FRAMEBUFFER);

	//glm::vec2 windowSize = glm::vec2(windowSize.x, windowSize.y);
	downsampleShaderProgram->use();
	downsampleShaderProgram->set2fv("texRes", glm::value_ptr(windowSize));

	baseTexture->bind(baseTexture->getId());
	downsampleShaderProgram->set1i("texture", baseTexture->getId());
	downsampleShaderProgram->set1i("mipLevel", 0);


	for (int i = 0; i < bloomChain.size(); i++)
	{

		gl->glViewport(0, 0, bloomChain[i].size.x, bloomChain[i].size.y);

		bloomFramebuffer->attachTexture(GL_COLOR_ATTACHMENT0, bloomChain[i].texture, 0, -1);

		//std::cout << "before bind" << std::endl;
		screenQuadVAO->bind();
//		std::cout << "after bind" << std::endl;

		gl->glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		screenQuadVAO->unbind();
	//	std::cout << "after unbind" << std::endl;

		downsampleShaderProgram->set2fv("texRes", glm::value_ptr(windowSize));
		downsampleShaderProgram->set1i("texture", bloomChain[i].texture->getId());

		bloomChain[i].texture->bind(bloomChain[i].texture->getId());

		if(i == 0) downsampleShaderProgram->set1i("mipLevel", 1);
	}
	gl->glUseProgram(0);

	//upsampling

	float bloomFilterRadius = 0.005f;

	upsampleShaderProgram->use();
	upsampleShaderProgram->set1f("filterRadius", bloomFilterRadius);

	// Enable additive blending
	gl->glEnable(GL_BLEND);
	gl->glBlendFunc(GL_ONE, GL_ONE);
	gl->glBlendEquation(GL_FUNC_ADD);

	for (int i = bloomChain.size() - 1; i > 0; i--)
	{
		upsampleShaderProgram->set1i("texture", bloomChain[i].texture->getId());
		bloomChain[i].texture->bind(bloomChain[i].texture->getId());
		gl->glViewport(0, 0, bloomChain[i - 1].size.x, bloomChain[i - 1].size.y);

		bloomFramebuffer->attachTexture(GL_COLOR_ATTACHMENT0, bloomChain[i - 1].texture, 0, -1);

		screenQuadVAO->bind();
		gl->glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		screenQuadVAO->unbind();
	}


	// Disable additive blending
	gl->glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	gl->glDisable(GL_BLEND);
}