#include "examples/vtk/VtkLookupTableExample.h"

#include "examples/ExampleRegistry.h"
#include "global_headers.h"

namespace {
ExampleRegistrar s_lutReg(QStringLiteral("VtkLookupTableExample"), []() {
    return std::make_unique<VtkLookupTableExample>();
});
}

VtkLookupTableExample::VtkLookupTableExample()
{
    m_actor = vtkSmartPointer<vtkActor>::New();
    m_grid = vtkSmartPointer<vtkUnstructuredGrid>::New();
}

ExampleMetadata VtkLookupTableExample::metadata() const
{
    ExampleMetadata meta;
    meta.id = QStringLiteral("vtk_lut");
    meta.title = QStringLiteral("Custom Lookup Table");
    meta.categoryId = QStringLiteral("vtk_basics");
    meta.summary = QStringLiteral("Populate a polygonal grid and map scalar values through a lookup table.");
    meta.tags = QStringList{QStringLiteral("vtk"), QStringLiteral("colors")};
    return meta;
}

void VtkLookupTableExample::run(const QVariantMap&, ExampleContext& context)
{
    context.clearScene();

    vtkNew<vtkPoints> points;
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

    m_grid->SetPoints(points);

    vtkSmartPointer<vtkIdList> pointsId = vtkSmartPointer<vtkIdList>::New();
    vtkIdType list[] = {9, 13, 8};
    m_grid->InsertNextCell(VTK_TRIANGLE, 3, list);

    pointsId->Initialize();
    pointsId->SetNumberOfIds(3);
    pointsId->SetId(0, 14);
    pointsId->SetId(1, 12);
    pointsId->SetId(2, 16);
    m_grid->InsertNextCell(VTK_TRIANGLE, 3, pointsId->GetPointer(0));

    pointsId->Initialize();
    pointsId->SetNumberOfIds(3);
    pointsId->SetId(0, 11);
    pointsId->SetId(1, 15);
    pointsId->SetId(2, 10);
    m_grid->InsertNextCell(VTK_TRIANGLE, 3, pointsId->GetPointer(0));

    pointsId->Initialize();
    pointsId->SetNumberOfIds(3);
    pointsId->SetId(0, 15);
    pointsId->SetId(1, 18);
    pointsId->SetId(2, 16);
    m_grid->InsertNextCell(VTK_TRIANGLE, 3, pointsId->GetPointer(0));

    pointsId->Initialize();
    pointsId->SetNumberOfIds(3);
    pointsId->SetId(0, 17);
    pointsId->SetId(1, 19);
    pointsId->SetId(2, 20);
    m_grid->InsertNextCell(VTK_TRIANGLE, 3, pointsId->GetPointer(0));

    pointsId->Initialize();
    pointsId->SetNumberOfIds(3);
    pointsId->SetId(0, 3);
    pointsId->SetId(1, 22);
    pointsId->SetId(2, 2);
    m_grid->InsertNextCell(VTK_TRIANGLE, 3, pointsId->GetPointer(0));

    pointsId->Initialize();
    pointsId->SetNumberOfIds(4);
    pointsId->SetId(0, 8);
    pointsId->SetId(1, 13);
    pointsId->SetId(2, 12);
    pointsId->SetId(3, 7);
    m_grid->InsertNextCell(VTK_QUAD, 4, pointsId->GetPointer(0));

    pointsId->Initialize();
    pointsId->SetNumberOfIds(4);
    pointsId->SetId(0, 12);
    pointsId->SetId(1, 14);
    pointsId->SetId(2, 6);
    pointsId->SetId(3, 7);
    m_grid->InsertNextCell(VTK_QUAD, 4, pointsId->GetPointer(0));

    pointsId->Initialize();
    pointsId->SetNumberOfIds(4);
    pointsId->SetId(0, 13);
    pointsId->SetId(1, 9);
    pointsId->SetId(2, 10);
    pointsId->SetId(3, 15);
    m_grid->InsertNextCell(VTK_QUAD, 4, pointsId->GetPointer(0));

    pointsId->Initialize();
    pointsId->SetNumberOfIds(4);
    pointsId->SetId(0, 6);
    pointsId->SetId(1, 14);
    pointsId->SetId(2, 17);
    pointsId->SetId(3, 5);
    m_grid->InsertNextCell(VTK_QUAD, 4, pointsId->GetPointer(0));

    pointsId->Initialize();
    pointsId->SetNumberOfIds(4);
    pointsId->SetId(0, 16);
    pointsId->SetId(1, 12);
    pointsId->SetId(2, 13);
    pointsId->SetId(3, 15);
    m_grid->InsertNextCell(VTK_QUAD, 4, pointsId->GetPointer(0));

    pointsId->Initialize();
    pointsId->SetNumberOfIds(4);
    pointsId->SetId(0, 14);
    pointsId->SetId(1, 16);
    pointsId->SetId(2, 19);
    pointsId->SetId(3, 17);
    m_grid->InsertNextCell(VTK_QUAD, 4, pointsId->GetPointer(0));

    pointsId->Initialize();
    pointsId->SetNumberOfIds(4);
    pointsId->SetId(0, 17);
    pointsId->SetId(1, 20);
    pointsId->SetId(2, 4);
    pointsId->SetId(3, 5);
    m_grid->InsertNextCell(VTK_QUAD, 4, pointsId->GetPointer(0));

    pointsId->Initialize();
    pointsId->SetNumberOfIds(4);
    pointsId->SetId(0, 16);
    pointsId->SetId(1, 18);
    pointsId->SetId(2, 21);
    pointsId->SetId(3, 19);
    m_grid->InsertNextCell(VTK_QUAD, 4, pointsId->GetPointer(0));

    pointsId->Initialize();
    pointsId->SetNumberOfIds(4);
    pointsId->SetId(0, 15);
    pointsId->SetId(1, 11);
    pointsId->SetId(2, 0);
    pointsId->SetId(3, 18);
    m_grid->InsertNextCell(VTK_QUAD, 4, pointsId->GetPointer(0));

    pointsId->Initialize();
    pointsId->SetNumberOfIds(4);
    pointsId->SetId(0, 20);
    pointsId->SetId(1, 19);
    pointsId->SetId(2, 21);
    pointsId->SetId(3, 22);
    m_grid->InsertNextCell(VTK_QUAD, 4, pointsId->GetPointer(0));

    pointsId->Initialize();
    pointsId->SetNumberOfIds(4);
    pointsId->SetId(0, 22);
    pointsId->SetId(1, 3);
    pointsId->SetId(2, 4);
    pointsId->SetId(3, 20);
    m_grid->InsertNextCell(VTK_QUAD, 4, pointsId->GetPointer(0));

    pointsId->Initialize();
    pointsId->SetNumberOfIds(4);
    pointsId->SetId(0, 21);
    pointsId->SetId(1, 18);
    pointsId->SetId(2, 0);
    pointsId->SetId(3, 1);
    m_grid->InsertNextCell(VTK_QUAD, 4, pointsId->GetPointer(0));

    pointsId->Initialize();
    pointsId->SetNumberOfIds(4);
    pointsId->SetId(0, 2);
    pointsId->SetId(1, 22);
    pointsId->SetId(2, 21);
    pointsId->SetId(3, 1);
    m_grid->InsertNextCell(VTK_QUAD, 4, pointsId->GetPointer(0));

    vtkSmartPointer<vtkFloatArray> color = vtkSmartPointer<vtkFloatArray>::New();
    for (int i = 0; i < m_grid->GetNumberOfCells(); ++i) {
        color->InsertNextValue(i);
    }
    m_grid->GetCellData()->SetScalars(color);

    vtkSmartPointer<vtkCellDataToPointData> toPoint = vtkSmartPointer<vtkCellDataToPointData>::New();
    toPoint->SetInputData(m_grid);
    toPoint->PassCellDataOn();
    toPoint->Update();

    const double rangeMin = toPoint->GetOutput()->GetScalarRange()[0];
    const double rangeMax = toPoint->GetOutput()->GetScalarRange()[1];

    vtkNew<vtkDataSetMapper> mapper;
    mapper->SetInputData(toPoint->GetOutput());
    mapper->SetScalarModeToUsePointData();
    mapper->SetScalarRange(rangeMin, rangeMax);

    m_actor->SetMapper(mapper);

    auto* renderer = context.renderer();
    renderer->AddActor(m_actor);
    renderer->ResetCamera();

    context.renderWindow()->Render();
    if (context.widget()) {
        context.widget()->update();
    }
}
