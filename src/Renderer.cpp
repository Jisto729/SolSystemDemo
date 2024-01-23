#include "Renderer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Scene.h"

//TODO remove probably
#include <geUtil/Text.h>
#include <glm/gtx/string_cast.hpp>
#include "Camera.h"

//#include <stb_image.h>

#include "SunTexture.h"

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

	std::shared_ptr<ssl::SunTexture> textureObject = scene->getLightObjects()[0].getTexture();
	std::vector<std::vector<float>> texVector = textureObject->getCurrentTexture();

	texture = std::make_shared<ge::gl::Texture>(GL_TEXTURE_CUBE_MAP, GL_RGBA, 0, textureObject->getWidth(), textureObject->getHeight());
	std::cout << texture->getId() << std::endl;
	std::cout << texture->getInfo() << std::endl;

	texture->bind(texture->getId());

	texture->setData2D(texVector[0].data(), GL_RGB, GL_FLOAT, 0, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, 0, textureObject->getWidth(), textureObject->getHeight(), 0);
	texture->setData2D(texVector[1].data(), GL_RGB, GL_FLOAT, 0, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, 0, textureObject->getWidth(), textureObject->getHeight(), 0);
	texture->setData2D(texVector[2].data(), GL_RGB, GL_FLOAT, 0, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, 0, textureObject->getWidth(), textureObject->getHeight(), 0);
	texture->setData2D(texVector[3].data(), GL_RGB, GL_FLOAT, 0, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, 0, textureObject->getWidth(), textureObject->getHeight(), 0);
	texture->setData2D(texVector[5].data(), GL_RGB, GL_FLOAT, 0, GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, 0, textureObject->getWidth(), textureObject->getHeight(), 0);
	texture->setData2D(texVector[4].data(), GL_RGB, GL_FLOAT, 0, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, 0, textureObject->getWidth(), textureObject->getHeight(), 0);

	texture->generateMipmap();

	std::cout << texture->getInfo() << texture->getId() << std::endl;
	std::cout << texture->getId() << std::endl;

	//TODO get from objects
	lightShaderProgram->setMatrix4fv("camMatrix", glm::value_ptr(matrix))
		->set1i("lightTexture", texture->getId())
		->bindBuffer("perDrawData_t", lightSSBO);

	lightVAO = std::make_shared<ge::gl::VertexArray>();

	lightVAO->addElementBuffer(scene->getIndicesBuff(lightObject));
	lightVAO->addAttrib(scene->getVerticesBuff(lightObject), 0, 3, GL_FLOAT);
	lightVAO->addAttrib(scene->getColorsBuff(lightObject), 1, 3, GL_FLOAT);
	lightVAO->addAttrib(scene->getModelIDsBuff(lightObject), 2, 1, GL_FLOAT);
	lightVAO->addAttrib(scene->getTexBuff(), 3, 3, GL_FLOAT);
	texture->unbind(texture->getId());

}

void Renderer::render(std::shared_ptr<Scene> scene)
{
	//TODO create a class for all of this
	texture->bind(texture->getId());
	std::shared_ptr<ssl::SunTexture> textureObject = scene->getLightObjects()[0].getTexture();
	std::vector<std::vector<float>> texVector = textureObject->getCurrentTexture();
	texture->setData2D(texVector[0].data(), GL_RGB, GL_FLOAT, 0, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, 0, textureObject->getWidth(), textureObject->getHeight(), 0);
	texture->setData2D(texVector[1].data(), GL_RGB, GL_FLOAT, 0, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, 0, textureObject->getWidth(), textureObject->getHeight(), 0);
	texture->setData2D(texVector[2].data(), GL_RGB, GL_FLOAT, 0, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, 0, textureObject->getWidth(), textureObject->getHeight(), 0);
	texture->setData2D(texVector[3].data(), GL_RGB, GL_FLOAT, 0, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, 0, textureObject->getWidth(), textureObject->getHeight(), 0);
	texture->setData2D(texVector[5].data(), GL_RGB, GL_FLOAT, 0, GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, 0, textureObject->getWidth(), textureObject->getHeight(), 0);
	texture->setData2D(texVector[4].data(), GL_RGB, GL_FLOAT, 0, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, 0, textureObject->getWidth(), textureObject->getHeight(), 0);
	texture->generateMipmap();

	gl->glViewport(0, 0, windowWidth * retinaScale, windowHeight * retinaScale);
	gl->glClearColor(0.0, 0.0, 0.0, 1.0);
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
	gl->glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	gl->glDrawElements(GL_TRIANGLES, scene->getIndicesBuff(normalObject)->getSize(), GL_UNSIGNED_INT, nullptr);

	lightShaderProgram->setMatrix4fv("camMatrix", glm::value_ptr(camera->getMatrix()))
		->set1i("lightTexture", texture->getId())
		->bindBuffer("perDrawData_t", lightSSBO);
	lightShaderProgram->use();
	lightVAO->bind();
	gl->glEnable(GL_DEPTH_TEST);
	gl->glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	gl->glDrawElements(GL_TRIANGLES, scene->getIndicesBuff(lightObject)->getSize(), GL_UNSIGNED_INT, nullptr);
	texture->unbind(texture->getId());

}