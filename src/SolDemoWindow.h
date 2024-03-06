#pragma once

//#include <QtGui/QWindow>
#include <QOpenGLWidget>
#include <QPushButton>
#include <geGL/geGL.h>
#include <memory>
#include "Camera.h"
#include "Renderer.h"
#include "EventHandler.h"
#include "Scene.h"
#include "GenAttributes.h"


namespace ssd
    {

    class SolDemoWindow : public QOpenGLWidget
    {
        Q_OBJECT
    public:
        explicit SolDemoWindow(QWidget* parent = 0);

        ~SolDemoWindow();

        //old
        //virtual void render();
        //virtual void initialize();

        //new
        virtual void paintGL();
        virtual void initializeGL();
        virtual void resizeGL(int width, int height);

        void generate(GenAttributes attribs);

        //TODO move to a higher class probably
        inline std::shared_ptr<Camera> getCameraPtr() { return camera; };

        static const std::string fragmentShaderSrc;


    //public slots:

        //void renderNow();

    protected:
        void animate();
        //bool event(QEvent* event) override;
        void keyPressEvent(QKeyEvent* event) override;
        void keyReleaseEvent(QKeyEvent* event) override;
        void mousePressEvent(QMouseEvent* event) override;
        void mouseReleaseEvent(QMouseEvent* event) override;
        void mouseMoveEvent(QMouseEvent* event) override;
        void timerEvent(QTimerEvent* event) override;
        //void exposeEvent(QExposeEvent* event) override;

    private:
        bool initialized;

        std::shared_ptr<Camera> camera;
        std::shared_ptr<Renderer> renderer;
        std::shared_ptr<EventHandler> eventHandler;
        std::shared_ptr<Scene> scene;

        int timer;
        //bool isMousePressed = false;
        //int mouseX;
        //int mouseY;
      //  QOpenGLContext* context;
        int paintNumber = 0;
        QSurfaceFormat surfaceFormat;
    };
}
