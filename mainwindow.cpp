#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
    ui->openGLWidget->setRenderWindow(renderWindow);
}

MainWindow::~MainWindow()
{
    delete ui;
}
