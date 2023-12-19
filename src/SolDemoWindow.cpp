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
ssd::SolDemoWindow::SolDemoWindow(QWindow* parent)
	: QWindow(parent)
	, initialized(false)
	, context(nullptr)
{
	setSurfaceType(QWindow::OpenGLSurface); //this needs to be set otherwise makeCurrent and other gl context related functions will fail
	surfaceFormat.setVersion(4, 5);
	surfaceFormat.setProfile(QSurfaceFormat::CoreProfile);

	//Timer
	//timer = new QTimer(this);
	timer = startTimer(1000);
}
//! [ctor]

ssd::SolDemoWindow::~SolDemoWindow()
{
	killTimer(timer);
}

void ssd::SolDemoWindow::initialize()
{
	if (initialized) return;
	//! [qt_context]
	if (!context)
	{
		context = new QOpenGLContext(this);
		context->setFormat(surfaceFormat);
		bool success = context->create();
		if (!success)
		{
			//TODO better exit
			exit(-1);
		}
	}
	//! [qt_context]

	//! [makeCurrent]
	//let's say to the OS that we want to work with this context
	context->makeCurrent(this);
	//! [makeCurrent]

	//! [geGL_init]
	/*GPUEngine Init*/
	ge::gl::init();

	camera = std::make_shared<Camera>(glm::vec3(2.0f, 0.0f, 0.0f), 50.0f, 0.1f, 100.0f);
	camera->updateMatrix(width(), height());
	scene = std::make_shared<Scene>();
	renderer = std::make_shared<Renderer>(context, width(), height(), devicePixelRatio(), camera, scene);

	eventHandler = std::make_shared<EventHandler>(camera);
	initialized = true;
}

//! [render]
void ssd::SolDemoWindow::render()
{
	renderer->render(scene);
	context->swapBuffers(this);
}
//! [render]

//! [renderNow]
void ssd::SolDemoWindow::renderNow()
{
	if (!isExposed()) return;
	if (!initialized) initialize();

	context->makeCurrent(this);
	//context never got released so no need to make it current again

	render();
}
//! [renderNow]


//events
void ssd::SolDemoWindow::keyPressEvent(QKeyEvent* event)
{
	if (eventHandler->event(event) == true)
	{
		renderNow();
	}
}

void ssd::SolDemoWindow::mouseMoveEvent(QMouseEvent* event)
{
	if (eventHandler->event(event, width(), height()) == true)
	{
		QCursor cursor = QCursor(Qt::BlankCursor);
		cursor.setPos(geometry().center());
		setCursor(cursor);
		renderNow();
	}
}

void ssd::SolDemoWindow::timerEvent(QTimerEvent* event)
{
	animate();
}

bool ssd::SolDemoWindow::event(QEvent* event)
{

	//class eventhandler, function will be called, if true, rerenders, if not, nothing
	if (eventHandler->event(event) == true)
	{
		renderNow();
	} else
	{
		return QWindow::event(event);
	}
}
//! [eventFilter]

//! [expose]
void ssd::SolDemoWindow::exposeEvent(QExposeEvent* event)
{
	if (isExposed())
	{
		renderNow();
	}
}
//! [expose]

void ssd::SolDemoWindow::animate()
{
		scene->moveObjects();
		renderNow();
}