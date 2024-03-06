
#include <QtGui/QGuiApplication>
#include <SolDemoWindow.h>
#include <AppWindow.h>
#include <QtGui/QOpenGLContext>
#include <QApplication>

//! [main]
int main(int argc, char** argv)
{
	QApplication app(argc, argv);

	ssd::AppWindow appWind;
	appWind.resize(1000, 800);
	appWind.show();

	return app.exec();
}
//! [main]