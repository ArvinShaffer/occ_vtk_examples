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
        "../..",
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

    //vtkSmartPointer<vtkRenderer> ren1 = vtkSmartPointer<vtkRenderer>::New();
    main_render_3d->AddActor(coneActor);
    main_render_3d->SetBackground(1.0, 0.0, 0.0);
    main_render_3d->SetViewport(0.0, 0.0, 0.5, 0.5);

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

    renWin3d->AddRenderer(main_render_3d);
    renWin3d->AddRenderer(ren2);
    renWin3d->AddRenderer(ren3);
    renWin3d->AddRenderer(ren4);
    renWin3d->Render();
    ui->openGLWidget->update();
}

void MainWindow::on_ex04volume_clicked()
{
    QString vtkPath = QFileDialog::getOpenFileName(
        this,
        tr("open vtk file"),
        "../../",
        tr("vtk file (*.vtk)"));
    if (vtkPath.isEmpty()) {
        return;
    }
    main_render_3d->RemoveAllViewProps();

    vtkSmartPointer<vtkStructuredPointsReader> vtkReader = vtkSmartPointer<vtkStructuredPointsReader>::New();
    vtkReader->SetFileName(vtkPath.toUtf8().constData());
    vtkReader->Update();
    //vtkVolumeRayCastMapper定义了一个光线投影体绘制Mapper
    vtkSmartPointer<vtkFixedPointVolumeRayCastMapper> volumeMapper = vtkSmartPointer<vtkFixedPointVolumeRayCastMapper>::New();
    volumeMapper->SetInputData(vtkReader->GetOutput());

    //设置光线采样距离
    //volumeMapper->SetSampleDistance(volumeMapper->GetSampleDistance()*4);
    //设置图像采样步长
    //volumeMapper->SetAutoAdjustSampleDistances(0);
    //volumeMapper->SetImageSampleDistance(4);

    vtkSmartPointer<vtkVolumeProperty> volumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();
    volumeProperty->SetInterpolationTypeToLinear();
    volumeProperty->ShadeOn(); //打开或者关闭阴影测试
    volumeProperty->SetAmbient(0.4);
    volumeProperty->SetDiffuse(0.6); //漫反射
    volumeProperty->SetSpecular(0.2); //镜面反射
    //设置不透明度
    vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
    compositeOpacity->AddPoint(70, 0.00);
    compositeOpacity->AddPoint(90, 0.40);
    compositeOpacity->AddPoint(180, 0.60);
    volumeProperty->SetScalarOpacity(compositeOpacity); //设置不透明度传输函数
    //compositeOpacity->AddPoint(120, 0.00);//测试隐藏部分数据,对比不同的设置
    //compositeOpacity->AddPoint(180, 0.60);
    //volumeProperty->SetScalarOpacity(compositeOpacity);
    //设置梯度不透明属性
    vtkSmartPointer<vtkPiecewiseFunction> volumeGradientOpacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
    volumeGradientOpacity->AddPoint(10, 0.0);
    volumeGradientOpacity->AddPoint(90, 0.5);
    volumeGradientOpacity->AddPoint(100, 1.0);
    volumeProperty->SetGradientOpacity(volumeGradientOpacity); //设置梯度不透明度效果对比
    //设置颜色属性
    vtkSmartPointer<vtkColorTransferFunction> color = vtkSmartPointer<vtkColorTransferFunction>::New();
    color->AddRGBPoint(0.000, 0.00, 0.00, 0.00);
    color->AddRGBPoint(64.00, 1.00, 0.52, 0.30);
    color->AddRGBPoint(190.0, 1.00, 1.00, 1.00);
    color->AddRGBPoint(220.0, 0.20, 0.20, 0.20);
    volumeProperty->SetColor(color);


    vtkSmartPointer<vtkVolume> volume = vtkSmartPointer<vtkVolume>::New();
    volume->SetMapper(volumeMapper);
    volume->SetProperty(volumeProperty);

    main_render_3d->SetBackground(0, 1, 0);
    main_render_3d->AddVolume(volume);

    renWin3d->Render();
    ui->openGLWidget->update();
}

void MainWindow::on_ex05points_clicked()
{
    main_render_3d->RemoveAllViewProps();
    vtkSmartPointer<vtkNamedColors> namedColors = vtkSmartPointer<vtkNamedColors>::New();
    //namedColors->SetColor("Bkg", 0.3, 0.6, 0.3);

    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    vtkIdType id1 = points->InsertNextPoint(0, 0, 0); //这种插入形式 不用添加id
    vtkIdType id2 = points->InsertNextPoint(1, 1, 1);
    vtkIdType id3 = points->InsertNextPoint(2, 2, 2);

    vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();
    polydata->SetPoints(points);

    //应用 vtkVertexGlyphFilter 在点周围制作单元格，vtk 只渲染单元格。
    vtkSmartPointer<vtkVertexGlyphFilter> vertexGlyphFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();
    vertexGlyphFilter->AddInputData(polydata);
    vertexGlyphFilter->Update();

    // Create a mapper and actor
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(vertexGlyphFilter->GetOutputPort());

    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    actor->GetProperty()->SetPointSize(100);
    actor->GetProperty()->SetRenderPointsAsSpheres(true);
    actor->GetProperty()->SetColor(namedColors->GetColor3d("Yellow").GetData());

    main_render_3d->AddActor(actor);
    main_render_3d->ResetCamera();
    renWin3d->Render();
    ui->openGLWidget->update();
}

