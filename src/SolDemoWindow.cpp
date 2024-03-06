#include "SolDemoWindow.h"

#include <QtGui/QOpenGLContext>
#include <QKeyEvent>
#include <geGL/geGL.h>
#include <geUtil/Text.h>
#include <geUtil/BasicCamera.h>
#include <geCore/ErrorPrinter.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <QCursor>

#include "Camera.h"
#include "Sphere.h"
#include "Renderer.h"
#include "EventHandler.h"
#include "Scene.h"

#include <iostream>


//! [ctor]
ssd::SolDemoWindow::SolDemoWindow(QWidget* parent)
	: QOpenGLWidget(parent)
	, initialized(false)
	//, context(nullptr)
{
	std::cout << "constructing" << std::endl;
	
	//old
	//setSurfaceType(QWindow::OpenGLSurface); //this needs to be set otherwise makeCurrent and other gl context related functions will fail
	/*surfaceFormat.setVersion(4, 5);
	surfaceFormat.setProfile(QSurfaceFormat::CoreProfile);*/

	//new surface
	surfaceFormat.setDepthBufferSize(24);
	surfaceFormat.setStencilBufferSize(8);
	surfaceFormat.setVersion(4, 5);
	surfaceFormat.setProfile(QSurfaceFormat::CoreProfile);
	setFormat(surfaceFormat);

	//Timer
	std::cout << "set Timer" << std::endl;
	timer = startTimer(20);
	//timer = startTimer(200);

	//mouseX = width() / 2.0;
	//mouseY = height() / 2.0;
	std::cout << "created renderwindow" << std::endl;
}
//! [ctor]

ssd::SolDemoWindow::~SolDemoWindow()
{
	killTimer(timer);
}

void ssd::SolDemoWindow::initializeGL()
{
	if (initialized) return;
	std::cout << "window initializing" << std::endl;
	
	//! [qt_context]
	//if (!context)
	//{
	//	context = new QOpenGLContext(this);
	//	context->setFormat(surfaceFormat);
	//	bool success = context->create();
	//	if (!success)
	//	{
	//		//TODO better exit
	//		exit(-1);
	//	}
	//}
	//! [qt_context]

	//! [makeCurrent]
	//let's say to the OS that we want to work with this context
	//context->makeCurrent(this);
	//! [makeCurrent]

	//! [geGL_init]
	/*GPUEngine Init*/
	ge::gl::init();

	camera = std::make_shared<Camera>(glm::vec3(0.0f, 2.0f, 10.0f), 50.0f, 0.1f, 100.0f);
	camera->updateMatrix(width(), height());
	scene = std::make_shared<Scene>();

	//renderer = std::make_shared<Renderer>(context, width(), height(), devicePixelRatio(), camera, scene);
	//renderer = std::make_shared<Renderer>(context(), width(), height(), devicePixelRatio(), camera, scene);

	renderer = std::make_shared<Renderer>(width(), height(), devicePixelRatio(), camera, scene, defaultFramebufferObject());

	eventHandler = std::make_shared<EventHandler>(camera);
	initialized = true;

	std::cout << "window initialized" << std::endl;
}

//! [render]
void ssd::SolDemoWindow::paintGL()
{
	//std::cout << "rendering" << std::endl;
	/*std::cout << paintNumber << std::endl;
	paintNumber++;*/
	//std::cout << "defFBO: " << defaultFramebufferObject() << std::endl;
	renderer->setDefaultFBO(defaultFramebufferObject());
	renderer->render(scene);
	//std::cout << "rendered" << std::endl;
	//QOpenGLContext::currentContext()->swapBuffers();
	//QOpenGLWidget::currentContext();
	//context->swapBuffers(this);
}
//! [render]

void ssd::SolDemoWindow::resizeGL(int width, int height)
{
	std::cout << "resizing" << std::endl;
	renderer->updateDimentions(width, height);
}

//! [renderNow]
//void ssd::SolDemoWindow::renderNow()
//{
//	//std::cout << "calling renderNow" << std::endl;
//	//if (!isExposed()) return;
//	if (!initialized) initializeGL();
//
//	//context->makeCurrent(this);
//	//context never got released so no need to make it current again
//
//	paintGL();
//}
//! [renderNow]

void ssd::SolDemoWindow::generate(GenAttributes attribs)
{
	scene->update(attribs);
}

//events
void ssd::SolDemoWindow::keyPressEvent(QKeyEvent* event)
{
	eventHandler->event(event);
}

void ssd::SolDemoWindow::keyReleaseEvent(QKeyEvent* event)
{
	eventHandler->event(event);
}

void ssd::SolDemoWindow::mousePressEvent(QMouseEvent* event)
{
	eventHandler->event(event);
	//isMousePressed = true;
}

void ssd::SolDemoWindow::mouseReleaseEvent(QMouseEvent* event)
{
	eventHandler->event(event);
//	isMousePressed = false;
}

void ssd::SolDemoWindow::mouseMoveEvent(QMouseEvent* event)
{
	eventHandler->event(event);
	//mouseX = event->pos().x();
	//mouseY = event->pos().y();
	//std::cout << mouseX << ", " << mouseY << std::endl;
	//QCursor cursor = QCursor(Qt::BlankCursor);
	//cursor.setPos(geometry().center());

	//if (eventHandler->checkFirstMove())
	//{
	//	std::cout << "first move" << std::endl;
	//	//setCursor(cursor);
	//	return;
	//}

	//if(eventHandler->event(event, width(), height()) == true)
	//{
	//	//setCursor(cursor);
	//}
}

void ssd::SolDemoWindow::timerEvent(QTimerEvent* event)
{
	//std::cout << "timer" << std::endl;
	if (eventHandler->isMousePressed())
	{
		eventHandler->handleMouse(width(), height());
	}
	camera->move();
	animate();
	update();
}

//! [expose]
//void ssd::SolDemoWindow::exposeEvent(QExposeEvent* event)
//{
//	if (isExposed())
//	{
//		renderNow();
//	}
//}
//! [expose]

void ssd::SolDemoWindow::animate()
{
	//std::cout << "calling animation" << std::endl;
	scene->moveObjects();
	scene->animateSurfaces();
	//renderNow();
}