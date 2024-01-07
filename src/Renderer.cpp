#include "Renderer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Scene.h"

//TODO remove probably
#include <geUtil/Text.h>
#include <glm/gtx/string_cast.hpp>
#include "Camera.h"


using namespace ssd;

Renderer::Renderer(QOpenGLContext* context, int width, int height, const qreal pixelRatio, std::shared_ptr<Camera> cameraPtr, std::shared_ptr<Scene> scene)
{
	//TODO probably add more classes to deal with the individual steps
	gl = std::make_shared<ge::gl::Context>();
	windowWidth = width;
	windowHeight = height;
	retinaScale = pixelRatio;
	camera = cameraPtr;

	//! [shader]
	std::shared_ptr<ge::gl::Shader> fragmentShader = std::make_shared<ge::gl::Shader>(GL_FRAGMENT_SHADER, ge::util::loadTextFile(FRAGMENT_SHADER));
	std::shared_ptr<ge::gl::Shader> vertexShader = std::make_shared<ge::gl::Shader>(GL_VERTEX_SHADER, ge::util::loadTextFile(VERTEX_SHADER));

	shaderProgram = std::make_shared<ge::gl::Program>(vertexShader, fragmentShader);

	//! [buffer_ctor]
	std::vector<glm::mat4> modelMatrices = scene->getModelMatrices(normalObject);
	SSBO = std::make_shared<ge::gl::Buffer>(modelMatrices.size() * sizeof(glm::mat4), modelMatrices.data());

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

	//![VAO]
	
	//lights
	std::shared_ptr<ge::gl::Shader> lightFragmentShader = std::make_shared<ge::gl::Shader>(GL_FRAGMENT_SHADER, ge::util::loadTextFile(LIGHT_FRAGMENT_SHADER));
	std::shared_ptr<ge::gl::Shader> lightVertexShader = std::make_shared<ge::gl::Shader>(GL_VERTEX_SHADER, ge::util::loadTextFile(LIGHT_VERTEX_SHADER));

	lightShaderProgram = std::make_shared<ge::gl::Program>(lightVertexShader, lightFragmentShader);

	modelMatrices = scene->getModelMatrices(lightObject);
	lightSSBO = std::make_shared<ge::gl::Buffer>(modelMatrices.size() * sizeof(glm::mat4), modelMatrices.data());

	glm::mat4 matrix = camera->getMatrix();

	//TODO get from objects
	lightShaderProgram->setMatrix4fv("camMatrix", glm::value_ptr(matrix))
		->bindBuffer("perDrawData_t", lightSSBO);

	lightVAO = std::make_shared<ge::gl::VertexArray>();

	lightVAO->addElementBuffer(scene->getIndicesBuff(lightObject));
	lightVAO->addAttrib(scene->getVerticesBuff(lightObject), 0, 3, GL_FLOAT);
	lightVAO->addAttrib(scene->getColorsBuff(lightObject), 1, 3, GL_FLOAT);
	lightVAO->addAttrib(scene->getModelIDsBuff(lightObject), 2, 1, GL_FLOAT);
}

void Renderer::render(std::shared_ptr<Scene> scene)
{
	gl->glViewport(0, 0, windowWidth * retinaScale, windowHeight * retinaScale);
	gl->glClearColor(.392, .584, 0.929, 1.0);
	gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	scene->updateModelMatrices(SSBO, normalObject);
	scene->updateModelMatrices(lightSSBO, lightObject);
	camera->updateMatrix(windowWidth, windowHeight);

	shaderProgram
		->setMatrix4fv("projectionMatrix", glm::value_ptr(camera->getProjectionMatrix()))
		->setMatrix4fv("viewMatrix", glm::value_ptr(camera->getViewMatrix()))
		->set3fv("lightPositions", glm::value_ptr(scene->getLightPositions()[0]), scene->getLightCount())
		->bindBuffer("perDrawData_t", SSBO);
	shaderProgram->use();
	VAO->bind();
	gl->glEnable(GL_DEPTH_TEST);
	gl->glDrawElements(GL_TRIANGLES, scene->getIndicesBuff(normalObject)->getSize(), GL_UNSIGNED_INT, nullptr);

	lightShaderProgram->setMatrix4fv("camMatrix", glm::value_ptr(camera->getMatrix()))
		->bindBuffer("perDrawData_t", lightSSBO);
	lightShaderProgram->use();
	lightVAO->bind();
	gl->glEnable(GL_DEPTH_TEST);
	gl->glDrawElements(GL_TRIANGLES, scene->getIndicesBuff(lightObject)->getSize(), GL_UNSIGNED_INT, nullptr);
}