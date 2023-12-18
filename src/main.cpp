
#include <QtGui/QGuiApplication>
#include <SolDemoWindow.h>
#include <QtGui/QOpenGLContext>

//! [main]
int main(int argc, char** argv)
{
	QGuiApplication app(argc, argv);

	ssd::SolDemoWindow window;
	window.resize(640, 480);
	window.show();

	return app.exec();
}
//! [main]