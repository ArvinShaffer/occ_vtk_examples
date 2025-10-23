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


void MainWindow::on_ex01_clicked()
{
    vtkSmartPointer<vtkCylinderSource> cylinder = vtkSmartPointer<vtkCylinderSource>::New();
    cylinder->SetCenter(0, 0, 0);
    cylinder->SetHeight(8.0);
    cylinder->SetRadius(3);
    cylinder->SetResolution(100);
    cylinder->Update();

    vtkSmartPointer<vtkCubeSource> cube = vtkSmartPointer<vtkCubeSource>::New();
    cube->SetCenter(5, 5, 5);

    vtkSmartPointer<vtkPolyDataMapper> cylinderMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    cylinderMapper->SetInputConnection(cylinder->GetOutputPort());

    vtkSmartPointer<vtkPolyData> data = cylinder->GetOutput();

    vtkSmartPointer<vtkPolyDataMapper> cubeMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    cubeMapper->SetInputConnection(cube->GetOutputPort());


    vtkSmartPointer<vtkActor> cylinderActor = vtkSmartPointer<vtkActor>::New();
    cylinderActor->SetMapper(cylinderMapper);
    cylinderActor->GetProperty()->SetOpacity(0.2);

    vtkSmartPointer<vtkActor> cubeActor = vtkSmartPointer<vtkActor>::New();
    cubeActor->SetMapper(cubeMapper);

    main_render_3d->RemoveAllViewProps();
    main_render_3d->AddActor(cylinderActor);
    main_render_3d->AddActor(cubeActor);
    main_render_3d->ResetCamera();
    renWin3d->Render();
    ui->openGLWidget->update();
}


void MainWindow::on_ex02light_clicked()
{
    // Light 1
    vtkSmartPointer<vtkLight> light1 = vtkSmartPointer<vtkLight>::New();
    light1->SetLightTypeToCameraLight();
    light1->SetColor(1, 3, 1);
    light1->SetPosition(0, 0, 1);
    light1->SetIntensity(0.8);
    light1->SetFocalPoint(main_render_3d->GetActiveCamera()->GetFocalPoint());
    main_render_3d->AddLight(light1);
    renWin3d->Render();
    ui->openGLWidget->update();
}


void MainWindow::on_ex03ViewPort_clicked()
{
    //main_render_3d->RemoveAllViewProps();
    vtkSmartPointer<vtkConeSource> cone = vtkSmartPointer<vtkConeSource>::New();
    vtkSmartPointer<vtkCubeSource> cube = vtkSmartPointer<vtkCubeSource>::New();
    vtkSmartPointer<vtkCylinderSource> cylinder = vtkSmartPointer<vtkCylinderSource>::New();
    vtkSmartPointer<vtkSphereSource> sphere = vtkSmartPointer<vtkSphereSource>::New();

    cone->Update();
    cube->Update();
    cylinder->Update();
    sphere->Update();

    vtkSmartPointer<vtkPolyDataMapper> coneMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    coneMapper->SetInputData(cone->GetOutput());
    vtkSmartPointer<vtkPolyDataMapper> cubeMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    cubeMapper->SetInputData(cube->GetOutput());
    vtkSmartPointer<vtkPolyDataMapper> cylinderMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    cylinderMapper->SetInputData(cylinder->GetOutput());
    vtkSmartPointer<vtkPolyDataMapper> sphereMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    sphereMapper->SetInputData(sphere->GetOutput());

    vtkSmartPointer<vtkActor> coneActor = vtkSmartPointer<vtkActor>::New();
    coneActor->SetMapper(coneMapper);
    vtkSmartPointer<vtkActor> cubeActor = vtkSmartPointer<vtkActor>::New();
    cubeActor->SetMapper(cubeMapper);
    vtkSmartPointer<vtkActor> cylinderActor = vtkSmartPointer<vtkActor>::New();
    cylinderActor->SetMapper(cylinderMapper);
    vtkSmartPointer<vtkActor> sphereActor = vtkSmartPointer<vtkActor>::New();
    sphereActor->SetMapper(sphereMapper);

    vtkSmartPointer<vtkRenderer> ren1 = vtkSmartPointer<vtkRenderer>::New();
    ren1->AddActor(coneActor);
    ren1->SetBackground(1.0, 0.0, 0.0);
    ren1->SetViewport(0.0, 0.0, 0.5, 0.5);

    vtkSmartPointer<vtkRenderer> ren2 = vtkSmartPointer<vtkRenderer>::New();
    ren2->AddActor(cubeActor);
    ren2->SetBackground(0.0, 1.0, 0.0);
    ren2->SetViewport(0.5, 0.0, 1.0, 0.5);

    vtkSmartPointer<vtkRenderer> ren3 = vtkSmartPointer<vtkRenderer>::New();
    ren3->AddActor(cylinderActor);
    ren3->SetBackground(0.0, 0.0, 1.0);
    ren3->SetViewport(0.0, 0.5, 0.5, 1.0);

    vtkSmartPointer<vtkRenderer> ren4 = vtkSmartPointer<vtkRenderer>::New();
    ren4->AddActor(sphereActor);
    ren4->SetBackground(1.0, 1.0, 0.0);
    ren4->SetViewport(0.5, 0.5, 1.0, 1.0);

    renWin3d->AddRenderer(ren1);
    renWin3d->AddRenderer(ren2);
    renWin3d->AddRenderer(ren3);
    renWin3d->AddRenderer(ren4);
    renWin3d->Render();
    ui->openGLWidget->update();
}






























