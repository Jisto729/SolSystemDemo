#include "SunRenderer.h"
#include <geUtil/Text.h>
#include <glm/gtc/type_ptr.hpp>

using namespace ssd;

SunRenderer::SunRenderer(std::shared_ptr<Scene> scene, std::shared_ptr<Camera> camera, std::shared_ptr<ge::gl::Context> gl)
	: scene(scene),
	camera(camera),
	gl(gl)
{
	std::shared_ptr<ge::gl::Shader> lightFragmentShader = std::make_shared<ge::gl::Shader>(GL_FRAGMENT_SHADER, ge::util::loadTextFile(LIGHT_FRAGMENT_SHADER));
	std::shared_ptr<ge::gl::Shader> lightVertexShader = std::make_shared<ge::gl::Shader>(GL_VERTEX_SHADER, ge::util::loadTextFile(LIGHT_VERTEX_SHADER));

	shaderProgram = std::make_shared<ge::gl::Program>(lightVertexShader, lightFragmentShader);

	std::vector<glm::mat4> modelMatrices = scene->getModelMatrices(lightObject);
	SSBO = std::make_shared<ge::gl::Buffer>(modelMatrices.size() * sizeof(glm::mat4), modelMatrices.data());

	glm::mat4 matrix = camera->getMatrix();

	scene->getLightObjects()[0].bindCurrentTexture();

	shaderProgram->setMatrix4fv("camMatrix", glm::value_ptr(matrix))
		->set1i("lightTexture", scene->getLightObjects()[0].getTexId())
		->bindBuffer("perDrawData_t", SSBO);


	VAO = std::make_shared<ge::gl::VertexArray>();

	VAO->addElementBuffer(scene->getIndicesBuff(lightObject));
	VAO->addAttrib(scene->getVerticesBuff(lightObject), 0, 3, GL_FLOAT);
	VAO->addAttrib(scene->getColorsBuff(lightObject), 1, 3, GL_FLOAT);
	VAO->addAttrib(scene->getModelIDsBuff(lightObject), 2, 1, GL_FLOAT);
	VAO->addAttrib(scene->getTexBuff(), 3, 3, GL_FLOAT);

	scene->getLightObjects()[0].unbindCurrentTexture();
}

void SunRenderer::render()
{
	scene->updateModelMatrices(SSBO, lightObject);
	scene->getLightObjects()[0].bindCurrentTexture();

	shaderProgram->setMatrix4fv("camMatrix", glm::value_ptr(camera->getMatrix()))
		->set1i("lightTexture", scene->getLightObjects()[0].getTexId())
		->bindBuffer("perDrawData_t", SSBO);
	shaderProgram->use();

	VAO->bind();
	gl->glDrawElements(GL_TRIANGLES, scene->getIndicesBuff(lightObject)->getSize(), GL_UNSIGNED_INT, nullptr);
	VAO->unbind();

	scene->getLightObjects()[0].unbindCurrentTexture();
}

