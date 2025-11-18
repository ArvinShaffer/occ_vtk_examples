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
    // vtkIdType id1 = points->InsertNextPoint(0, 0, 0); //这种插入形式 不用添加id
    // vtkIdType id2 = points->InsertNextPoint(1, 1, 1);
    // vtkIdType id3 = points->InsertNextPoint(2, 2, 2);

    points->InsertNextPoint(0, 0, 0);
    points->InsertNextPoint(1, 1, 1);
    points->InsertNextPoint(2, 2, 2);

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


void MainWindow::on_ex06scalarBar_clicked()
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

    vtkSmartPointer<vtkUnstructuredGridReader> vtkReader = vtkSmartPointer<vtkUnstructuredGridReader>::New();
    vtkReader->SetFileName(vtkPath.toUtf8().constData());
    vtkReader->Update();

    vtkSmartPointer<vtkLookupTable> lut = vtkSmartPointer<vtkLookupTable>::New();
    lut->Build();
    vtkSmartPointer<vtkDataSetMapper> mapper = vtkSmartPointer<vtkDataSetMapper>::New();
    mapper->SetInputData(vtkReader->GetOutput());
    mapper->SetScalarRange(vtkReader->GetOutput()->GetScalarRange());
    mapper->SetLookupTable(lut);

    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);

    main_render_3d->AddActor(actor);
    //main_render_3d->SetBackground(1, 1, 1);

    if (!m_scalarBarWidget) {
        m_scalarBarWidget = vtkSmartPointer<vtkScalarBarWidget>::New();
    }
    if (!m_scalarBarActor) {
        m_scalarBarActor = vtkSmartPointer<vtkScalarBarActor>::New();
    }

    m_scalarBarActor->SetOrientationToHorizontal();
    m_scalarBarActor->SetLookupTable(lut);
    m_scalarBarActor->DrawBackgroundOn();
    m_scalarBarActor->GetBackgroundProperty()->SetColor(0, 0, 0);

    m_scalarBarWidget->SetInteractor(ui->openGLWidget->interactor());
    m_scalarBarWidget->SetScalarBarActor(m_scalarBarActor);
    m_scalarBarWidget->EnabledOn();
    m_scalarBarWidget->RepositionableOn();
    //scalarBarWidget->On();


    //main_render_3d->AddActor(scalarBarActor);
    main_render_3d->ResetCamera();
    renWin3d->Render();
    ui->openGLWidget->update();
}


void MainWindow::on_ex07lut_clicked()
{
    /* 创建一个非结构化网格对象 */
    vtkSmartPointer<vtkUnstructuredGrid> grid = vtkSmartPointer<vtkUnstructuredGrid>::New();

    /* 网格节点 */
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();  /* 用于存储网格节点 */
    points->InsertNextPoint(20, 0, 0);
    points->InsertNextPoint(17.320508109792, 10.0000000321997, 0);
    points->InsertNextPoint(10.0000000084796, 17.3205081212641, 0);
    points->InsertNextPoint(0, 20, 0);
    points->InsertNextPoint(-10.0000000245946, 17.3205081142845, 0);
    points->InsertNextPoint(-17.320508106976, 10.0000000330863, 0);
    points->InsertNextPoint(-20, 0, 0);
    points->InsertNextPoint(-17.3205081361632, -9.99999998652343, 0);
    points->InsertNextPoint(-10.0000000640139, -17.3205080893384, 0);
    points->InsertNextPoint(0, -20, 0);
    points->InsertNextPoint(9.99999997546481, -17.3205081119126, 0);
    points->InsertNextPoint(17.3205080670077, -10.000000025522, 0);
    points->InsertNextPoint(-7.15966651257561, -7.12683602128246, 0);
    points->InsertNextPoint(-2.63739358105485, -12.9978938464381, 0);
    points->InsertNextPoint(-9.10592684278358, -0.737849316405346, 0);
    points->InsertNextPoint(6.59947131139743, -9.08284206416132, 0);
    points->InsertNextPoint(-0.137734590259206, -2.71811814272003, 0);
    points->InsertNextPoint(-7.75933677195531, 5.80947235781205, 0);
    points->InsertNextPoint(9.11726442880688, -1.21602962482667, 0);
    points->InsertNextPoint(-0.4843222809037511, 4.65580556604978, 0);
    points->InsertNextPoint(-3.48354780828141, 10.4142340529639, 0);
    points->InsertNextPoint(7.28614335171517, 6.31993800133739, 0);
    points->InsertNextPoint(3.36376652373578, 13.053010718073, 0);
    grid->SetPoints(points);

    /*==============================================================*/


    /*=============================设置单元===========================*/
    vtkSmartPointer<vtkIdList> pointsId = vtkSmartPointer<vtkIdList>::New();
    /*pointsId->SetNumberOfIds(3);
    pointsId->SetId(0, 9);
    pointsId->SetId(1, 13);
    pointsId->SetId(2, 8);
    grid->InsertNextCell(VTK_TRIANGLE, 3, pointsId->GetPointer(0));*/

    vtkIdType list[] = {9,13,8};
    grid->InsertNextCell(VTK_TRIANGLE, 3, list);

    pointsId->Initialize();
    pointsId->SetNumberOfIds(3);
    pointsId->SetId(0, 14);
    pointsId->SetId(1, 12);
    pointsId->SetId(2, 16);
    grid->InsertNextCell(VTK_TRIANGLE, 3, pointsId->GetPointer(0));

    pointsId->Initialize();
    pointsId->SetNumberOfIds(3);
    pointsId->SetId(0, 11);
    pointsId->SetId(1, 15);
    pointsId->SetId(2, 10);
    grid->InsertNextCell(VTK_TRIANGLE, 3, pointsId->GetPointer(0));

    pointsId->Initialize();
    pointsId->SetNumberOfIds(3);
    pointsId->SetId(0, 15);
    pointsId->SetId(1, 18);
    pointsId->SetId(2, 16);
    grid->InsertNextCell(VTK_TRIANGLE, 3, pointsId->GetPointer(0));

    pointsId->Initialize();
    pointsId->SetNumberOfIds(3);
    pointsId->SetId(0, 17);
    pointsId->SetId(1, 19);
    pointsId->SetId(2, 20);
    grid->InsertNextCell(VTK_TRIANGLE, 3, pointsId->GetPointer(0));

    pointsId->Initialize();
    pointsId->SetNumberOfIds(3);
    pointsId->SetId(0, 3);
    pointsId->SetId(1, 22);
    pointsId->SetId(2, 2);
    grid->InsertNextCell(VTK_TRIANGLE, 3, pointsId->GetPointer(0));

    pointsId->Initialize();
    pointsId->SetNumberOfIds(4);
    pointsId->SetId(0, 8);
    pointsId->SetId(1, 13);
    pointsId->SetId(2, 12);
    pointsId->SetId(3, 7);
    grid->InsertNextCell(VTK_QUAD, 4, pointsId->GetPointer(0));

    pointsId->Initialize();
    pointsId->SetNumberOfIds(4);
    pointsId->SetId(0, 12);
    pointsId->SetId(1, 14);
    pointsId->SetId(2, 6);
    pointsId->SetId(3, 7);
    grid->InsertNextCell(VTK_QUAD, 4, pointsId->GetPointer(0));

    pointsId->Initialize();
    pointsId->SetNumberOfIds(4);
    pointsId->SetId(0, 13);
    pointsId->SetId(1, 9);
    pointsId->SetId(2, 10);
    pointsId->SetId(3, 15);
    grid->InsertNextCell(VTK_QUAD, 4, pointsId->GetPointer(0));

    pointsId->Initialize();
    pointsId->SetNumberOfIds(4);
    pointsId->SetId(0, 6);
    pointsId->SetId(1, 14);
    pointsId->SetId(2, 17);
    pointsId->SetId(3, 5);
    grid->InsertNextCell(VTK_QUAD, 4, pointsId->GetPointer(0));

    pointsId->Initialize();
    pointsId->SetNumberOfIds(4);
    pointsId->SetId(0, 16);
    pointsId->SetId(1, 12);
    pointsId->SetId(2, 13);
    pointsId->SetId(3, 15);
    grid->InsertNextCell(VTK_QUAD, 4, pointsId->GetPointer(0));

    pointsId->Initialize();
    pointsId->SetNumberOfIds(4);
    pointsId->SetId(0, 14);
    pointsId->SetId(1, 16);
    pointsId->SetId(2, 19);
    pointsId->SetId(3, 17);
    grid->InsertNextCell(VTK_QUAD, 4, pointsId->GetPointer(0));

    pointsId->Initialize();
    pointsId->SetNumberOfIds(4);
    pointsId->SetId(0, 17);
    pointsId->SetId(1, 20);
    pointsId->SetId(2, 4);
    pointsId->SetId(3, 5);
    grid->InsertNextCell(VTK_QUAD, 4, pointsId->GetPointer(0));

    pointsId->Initialize();
    pointsId->SetNumberOfIds(4);
    pointsId->SetId(0, 16);
    pointsId->SetId(1, 18);
    pointsId->SetId(2, 21);
    pointsId->SetId(3, 19);
    grid->InsertNextCell(VTK_QUAD, 4, pointsId->GetPointer(0));

    pointsId->Initialize();
    pointsId->SetNumberOfIds(4);
    pointsId->SetId(0, 15);
    pointsId->SetId(1, 11);
    pointsId->SetId(2, 0);
    pointsId->SetId(3, 18);
    grid->InsertNextCell(VTK_QUAD, 4, pointsId->GetPointer(0));

    pointsId->Initialize();
    pointsId->SetNumberOfIds(4);
    pointsId->SetId(0, 20);
    pointsId->SetId(1, 19);
    pointsId->SetId(2, 21);
    pointsId->SetId(3, 22);
    grid->InsertNextCell(VTK_QUAD, 4, pointsId->GetPointer(0));

    pointsId->Initialize();
    pointsId->SetNumberOfIds(4);
    pointsId->SetId(0, 22);
    pointsId->SetId(1, 3);
    pointsId->SetId(2, 4);
    pointsId->SetId(3, 20);
    grid->InsertNextCell(VTK_QUAD, 4, pointsId->GetPointer(0));

    pointsId->Initialize();
    pointsId->SetNumberOfIds(4);
    pointsId->SetId(0, 21);
    pointsId->SetId(1, 18);
    pointsId->SetId(2, 0);
    pointsId->SetId(3, 1);
    grid->InsertNextCell(VTK_QUAD, 4, pointsId->GetPointer(0));

    pointsId->Initialize();
    pointsId->SetNumberOfIds(4);
    pointsId->SetId(0, 2);
    pointsId->SetId(1, 22);
    pointsId->SetId(2, 21);
    pointsId->SetId(3, 1);
    grid->InsertNextCell(VTK_QUAD, 4, pointsId->GetPointer(0));

    /*==================================================设置单元颜色==================================================*/
    vtkSmartPointer<vtkFloatArray> color = vtkSmartPointer<vtkFloatArray>::New(); /* 用于设置每个单元的随机颜色 */
    for (int i = 0; i < grid->GetNumberOfCells(); ++i) {
        //color->InsertNextValue(std::rand());
        color->InsertNextValue(i);
    }
    grid->GetCellData()->SetScalars(color); /* 设置单元格的颜色 */

    /*==================================================单元---->节点=============================================*/
    double rangeMin = grid->GetScalarRange()[0];
    double rangeMax = grid->GetScalarRange()[1];

    vtkSmartPointer<vtkCellDataToPointData> theCellDataToPointData = vtkSmartPointer<vtkCellDataToPointData>::New();
    theCellDataToPointData->SetInputData(grid);
    /*
控制是否将输入单元数据传递到输出。如果on，然后将输入单元格数据传递到输出；否则，
仅将生成的点数据放入输出中。
*/
    theCellDataToPointData->PassCellDataOn();
    theCellDataToPointData->Update();

    // grid = (vtkUnstructuredGrid*)theCellDataToPointData->GetOutput();
    // rangeMin = grid->GetScalarRange()[0];
    // rangeMax = grid->GetScalarRange()[1];

    /*===========================================================================================================*/
    vtkSmartPointer<vtkDataSetMapper> mapper = vtkSmartPointer<vtkDataSetMapper>::New();
    mapper->SetInputData(grid);
    mapper->SetInputData(theCellDataToPointData->GetOutput());

    mapper->SetScalarModeToUsePointData();
    mapper->SetScalarRange(rangeMin, rangeMax);

    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    // actor->GetProperty()->EdgeVisibilityOn();

    main_render_3d->AddActor(actor);
    main_render_3d->ResetCamera();
    renWin3d->Render();
    ui->openGLWidget->update();

}

