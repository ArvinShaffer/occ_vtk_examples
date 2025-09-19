#include "icpprepost.h"
#include <QMenuBar>
#include <QStatusBar>


ICPPrePost::ICPPrePost(QWidget* parent) : QMainWindow(parent)
{
    setWindowTitle("ICPPrePost");
    resize(1200, 1200);

    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    openGLWidget = new QVTKOpenGLNativeWidget(centralWidget);
    openGLWidget->setGeometry(60, 40, 1100, 1100);

    menubar = new QMenuBar(this);
    menubar->setGeometry(QRect(0, 0, 800, 22));
    setMenuBar(menubar);

    statusbar = new QStatusBar(this);
    setStatusBar(statusbar);

    BRepPrimAPI_MakeBox mkBox(1., 2., 3);
    const TopoDS_Shape& shape = mkBox.Shape();
    vtkNew<vtkNamedColors> colors;
    vtkNew<IVtkTools_ShapeDataSource> occSource;
    occSource->SetShape(new IVtkOCC_Shape(shape));

    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputConnection(occSource->GetOutputPort());

    vtkNew<vtkActor> actor;
    actor->SetMapper(mapper);

    vtkNew<vtkRenderer> renderer;
    renderer->AddActor(actor);
    renderer->SetBackground(colors->GetColor3d("SteelBlue").GetData());

    vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
    renderWindow->AddRenderer(renderer);
    renderWindow->SetWindowName("icpprepost");
    openGLWidget->setRenderWindow(renderWindow);
}

ICPPrePost::~ICPPrePost()
{

}
