
#include <QtGui/QGuiApplication>
#include <SolDemoWindow.h>
#include <QtGui/QOpenGLContext>

//! [main]
int main(int argc, char** argv)
{
	QGuiApplication app(argc, argv);

	ssd::SolDemoWindow window;
	window.resize(800, 800);
	window.show();

	return app.exec();
}
//! [main]