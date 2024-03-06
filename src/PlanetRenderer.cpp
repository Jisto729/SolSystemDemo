#include "PlanetRenderer.h"
#include <geUtil/Text.h>
#include <glm/gtc/type_ptr.hpp>

using namespace ssd;

PlanetRenderer::PlanetRenderer(std::shared_ptr<Scene> scene, std::shared_ptr<Camera> camera, std::shared_ptr<ge::gl::Context> gl)
	: scene(scene),
	camera(camera),
	gl(gl)
{
	std::shared_ptr<ge::gl::Shader> fragmentShader = std::make_shared<ge::gl::Shader>(GL_FRAGMENT_SHADER, ge::util::loadTextFile(FRAGMENT_SHADER));
	std::shared_ptr<ge::gl::Shader> vertexShader = std::make_shared<ge::gl::Shader>(GL_VERTEX_SHADER, ge::util::loadTextFile(VERTEX_SHADER));

	shaderProgram = std::make_shared<ge::gl::Program>(vertexShader, fragmentShader);

	//! [buffer_ctor]
	std::vector<glm::mat4> modelMatrices = scene->getModelMatrices(normalObject);
	SSBO = std::make_shared<ge::gl::Buffer>(modelMatrices.size() * sizeof(glm::mat4), modelMatrices.data());
	std::cout << "SSBO size: " << SSBO->getSize() << std::endl;

	// Uniforms
	glm::mat4 projectionMatrix = camera->getProjectionMatrix();
	glm::mat4 viewMatrix = camera->getViewMatrix();

	std::vector<glm::vec3> lightPositions = scene->getLightPositions();
	std::vector<glm::vec3> lightColors = scene->getLightColors();
	glm::vec3 camPosition = camera->getPosition();
	int lightCount = scene->getLightCount();

	shaderProgram
		->setMatrix4fv("projectionMatrix", glm::value_ptr(projectionMatrix))
		->setMatrix4fv("viewMatrix", glm::value_ptr(viewMatrix))
		->set3fv("lightPositions", glm::value_ptr(lightPositions[0]), lightCount)
		->set3fv("lightColors", glm::value_ptr(lightColors[0]), lightCount)
		->set3fv("camPosition", glm::value_ptr(camPosition))
		->set1i("lightCount", lightCount)
		->bindBuffer("perDrawData_t", SSBO);

	//! [VAO]

	VAO = std::make_shared<ge::gl::VertexArray>();

	VAO->addElementBuffer(scene->getIndicesBuff(normalObject));
	VAO->addAttrib(scene->getVerticesBuff(normalObject), 0, 3, GL_FLOAT);
	VAO->addAttrib(scene->getColorsBuff(normalObject), 1, 3, GL_FLOAT);
	VAO->addAttrib(scene->getModelIDsBuff(normalObject), 2, 1, GL_FLOAT);
	VAO->addAttrib(scene->getNormalsBuff(), 3, 3, GL_FLOAT);
}

void PlanetRenderer::render()
{
	scene->updateModelMatrices(SSBO, normalObject);
	//scene->updateModelMatrices(lightSSBO, lightObject);

	shaderProgram
		->setMatrix4fv("projectionMatrix", glm::value_ptr(camera->getProjectionMatrix()))
		->setMatrix4fv("viewMatrix", glm::value_ptr(camera->getViewMatrix()))
		->set3fv("lightPositions", glm::value_ptr(scene->getLightPositions()[0]), scene->getLightCount())
		->bindBuffer("perDrawData_t", SSBO);
	shaderProgram->use();
	VAO->bind();

	gl->glDrawElements(GL_TRIANGLES, scene->getIndicesBuff(normalObject)->getSize(), GL_UNSIGNED_INT, nullptr);

	VAO->unbind();
}


