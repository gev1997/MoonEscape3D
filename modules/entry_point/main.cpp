// project
#include "moon_escape_3D_widget.h"

// third party
#include <QApplication>

// std

void init_surface_format()
{
	QSurfaceFormat format = QSurfaceFormat::defaultFormat();
    // format.setVersion(3, 3);
    // format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setSamples(4); // MSAA
    format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    format.setSwapInterval(1); // Enable VSync
    QSurfaceFormat::setDefaultFormat(format);
}

int main(int argc, char* argv[])
{
	init_surface_format();

	QApplication app(argc, argv);
    gui::moon_escape_3D_widget widget;
	widget.setFixedSize(800, 800);
    widget.show();

	return app.exec();
}
