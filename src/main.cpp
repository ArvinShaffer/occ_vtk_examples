#include <QApplication>
#include "icpprepost.h"

int main(int argc, char* argv[])
{
    QSurfaceFormat::setDefaultFormat(QVTKOpenGLNativeWidget::defaultFormat());
    QApplication a(argc, argv);
    ICPPrePost w;
    w.show();
    return a.exec();
}

