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

Renderer::Renderer(QOpenGLContext* context, int width, int height, const qreal pixelRatio, std::shared_ptr<Camera> cameraPtr) {
	gl = std::make_shared<ge::gl::Context>();
    windowWidth = width;
    windowHeight = height;
    retinaScale = pixelRatio;
    camera = cameraPtr;

    Scene scene = Scene();
    //TODO change to allow more than one object
    trianglePos = scene.getObjects()[0].getVertices();
    triangleCol = scene.getObjects()[0].getColors();
    indices = scene.getObjects()[0].getIndices();

    std::for_each(trianglePos.begin(), trianglePos.end(), [](auto& val) {std::cout << val << " "; });
    std::cout << std::endl;

    std::cout << "vertex shader: " << VERTEX_SHADER << std::endl;
    std::cout << "frag shader: " << FRAGMENT_SHADER << std::endl;

    //! [shader]
    std::shared_ptr<ge::gl::Shader> fragmentShader = std::make_shared<ge::gl::Shader>(GL_FRAGMENT_SHADER, ge::util::loadTextFile(FRAGMENT_SHADER));
    std::shared_ptr<ge::gl::Shader> vertexShader = std::make_shared<ge::gl::Shader>(GL_VERTEX_SHADER, ge::util::loadTextFile(VERTEX_SHADER));
    shaderProgram = std::make_shared<ge::gl::Program>(vertexShader, fragmentShader);

    //TODO move to scene objects
    positions = std::make_shared<ge::gl::Buffer>(trianglePos.size() * sizeof(float), trianglePos.data()/*, GL_STATIC_DRAW */);
    colors = std::make_shared<ge::gl::Buffer>(triangleCol.size() * sizeof(float), triangleCol.data()/*, GL_STATIC_DRAW */);
    elementBuffer = std::make_shared<ge::gl::Buffer>(indices.size() * sizeof(int), indices.data()/*, GL_STATIC_DRAW */);

    //! [buffer_ctor]
    std::vector<glm::mat4> modelMatrices;
    glm::mat4 mat = glm::translate(glm::mat4(1.0f), glm::vec3{ 0,0,-5 }) * glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3{ 1.f,1.0f,0.f });

    modelMatrices.push_back(mat);

    SSBO = std::make_shared<ge::gl::Buffer>(modelMatrices.size() * sizeof(glm::mat4), modelMatrices.data());

    // Uniforms
    glm::mat4 matrix = camera->getMatrix();
    shaderProgram->setMatrix4fv("camMatrix", glm::value_ptr(matrix))
        ->bindBuffer("perDrawData_t", SSBO);

    //! [VAO]

    VAO = std::make_shared<ge::gl::VertexArray>();

    VAO->addElementBuffer(elementBuffer);
    VAO->addAttrib(positions, 0, 3, GL_FLOAT);
    VAO->addAttrib(colors, 1, 3, GL_FLOAT);
    //![VAO]
}


void Renderer::render() {
    gl->glViewport(0, 0, windowWidth * retinaScale, windowHeight * retinaScale);
    gl->glClearColor(.392, .584, 0.929, 1.0);
    gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    camera->updateMatrix(windowWidth, windowHeight);
    shaderProgram->setMatrix4fv("camMatrix", glm::value_ptr(camera->getMatrix()));

    shaderProgram->use();
    VAO->bind();
    gl->glEnable(GL_DEPTH_TEST);
    gl->glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
}
