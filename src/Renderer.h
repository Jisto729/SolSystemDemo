#pragma once 

#include <memory>
#include <vector>
#include <geGL/geGL.h>
#include <QtGui/QOpenGLContext>
#include "Camera.h"


namespace ssd {
	class Renderer {
    public:

        Renderer(QOpenGLContext* context, int width, int height, const qreal retinaScale, std::shared_ptr<Camera> cameraPtr);

        void render();
        void renderNow();
        //TODO add window resize function handler
	private:

        int windowWidth;
        int windowHeight;

        std::vector<float> trianglePos;
        std::vector<float> triangleCol;
        std::vector<int> indices;

        std::shared_ptr<ge::gl::Buffer> positions;
        std::shared_ptr<ge::gl::Buffer> colors;
        std::shared_ptr<ge::gl::Buffer> elementBuffer;

        std::shared_ptr<ge::gl::Context> gl;
        std::shared_ptr<ge::gl::Program> shaderProgram;
        std::shared_ptr<ge::gl::VertexArray> VAO;
        std::shared_ptr<ge::gl::Buffer> SSBO;

        QOpenGLContext* context;

        std::shared_ptr<Camera> camera;
        qreal retinaScale;
	};
}