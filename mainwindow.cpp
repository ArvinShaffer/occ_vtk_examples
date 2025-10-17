#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    renWin3d = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    current_actor = vtkSmartPointer<vtkActor>::New();

    main_render_3d = vtkSmartPointer<vtkRenderer>::New();
    ugrid = vtkSmartPointer<vtkUnstructuredGrid>::New();
    colors = vtkSmartPointer<vtkNamedColors>::New();
    main_render_3d->SetBackground(colors->GetColor3d("SteelBlue").GetData());

    renWin3d->AddRenderer(main_render_3d);
    ui->openGLWidget->setRenderWindow(renWin3d);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_load_clicked()
{
    QString vtkPath = QFileDialog::getOpenFileName(
        this,
        tr("open vtk file"),
        "..",
        tr("vtk file (*.vtk)"));
    if (vtkPath.isEmpty()) {
        return;
    }
    main_render_3d->RemoveAllViewProps();
    vtkSmartPointer<vtkUnstructuredGridReader> vtkReader = vtkSmartPointer<vtkUnstructuredGridReader>::New();
    vtkReader->SetFileName(vtkPath.toUtf8().constData());
    vtkReader->Update();
    ugrid = vtkReader->GetOutput();
    vtkSmartPointer<vtkDataSetMapper> skinMapper = vtkSmartPointer<vtkDataSetMapper>::New();
    skinMapper->SetInputData(ugrid);
    vtkSmartPointer<vtkActor> skin = vtkSmartPointer<vtkActor>::New();
    skin->SetMapper(skinMapper);

    //skin->GetProperty()->SetRepresentationToWireframe();

    main_render_3d->AddActor(skin);
    main_render_3d->ResetCamera();
    renWin3d->Render();
    ui->openGLWidget->update();
}


void MainWindow::on_btn_occbox_clicked()
{
    main_render_3d->RemoveAllViewProps();
    BRepPrimAPI_MakeBox mkBox(1., 2., 3);
    const TopoDS_Shape& shape = mkBox.Shape();
    //vtkNew<vtkNamedColors> colors;
    vtkNew<IVtkTools_ShapeDataSource> occSource;
    occSource->SetShape(new IVtkOCC_Shape(shape));

    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputConnection(occSource->GetOutputPort());
    current_actor->SetMapper(mapper);
    main_render_3d->AddActor(current_actor);
    main_render_3d->ResetCamera();
    renWin3d->Render();
    ui->openGLWidget->update();
}

