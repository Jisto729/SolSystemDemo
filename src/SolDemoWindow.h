#pragma once

#include <QtGui/QWindow>
#include <geGL/geGL.h>
#include <memory>
#include "Camera.h"
#include "Renderer.h"
#include "EventHandler.h"
#include "Scene.h"


namespace ssd
    {

    class SolDemoWindow : public QWindow
    {
        Q_OBJECT
    public:
        explicit SolDemoWindow(QWindow* parent = 0);

        ~SolDemoWindow();

        virtual void render();
        virtual void initialize();

        //TODO move to a higher class probably
        inline std::shared_ptr<Camera> getCameraPtr() { return camera; };

        static const std::string fragmentShaderSrc;

    public slots:

        void renderNow();

    protected:
        void animate();
        //bool event(QEvent* event) override;
        void keyPressEvent(QKeyEvent* event) override;
        void keyReleaseEvent(QKeyEvent* event) override;
        void mouseMoveEvent(QMouseEvent* event) override;
        void timerEvent(QTimerEvent* event) override;
        void exposeEvent(QExposeEvent* event) override;

    private:
        bool initialized;

        std::shared_ptr<Camera> camera;
        std::shared_ptr<Renderer> renderer;
        std::shared_ptr<EventHandler> eventHandler;
        std::shared_ptr<Scene> scene;

        int timer;
        QOpenGLContext* context;
        QSurfaceFormat surfaceFormat;
    };
}
