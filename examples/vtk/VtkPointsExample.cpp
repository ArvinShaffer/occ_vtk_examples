#include "examples/vtk/VtkPointsExample.h"

#include "examples/ExampleRegistry.h"
#include "global_headers.h"

namespace {
ExampleRegistrar s_pointsReg(QStringLiteral("VtkPointsExample"), []() {
    return std::make_unique<VtkPointsExample>();
});
}

VtkPointsExample::VtkPointsExample()
{
    m_pointsActor = vtkSmartPointer<vtkActor>::New();
}

ExampleMetadata VtkPointsExample::metadata() const
{
    ExampleMetadata meta;
    meta.id = QStringLiteral("vtk_points");
    meta.title = QStringLiteral("Glyph Points");
    meta.categoryId = QStringLiteral("vtk_basics");
    meta.summary = QStringLiteral("Convert points into renderable glyph cells and adjust style.");
    meta.tags = QStringList{QStringLiteral("vtk"), QStringLiteral("points")};
    return meta;
}

QList<ExampleParameter> VtkPointsExample::parameters() const
{
    return {
        ExampleParameter{
            QStringLiteral("size"),
            QStringLiteral("Point Size"),
            ExampleParameterType::Integer,
            12,
            1,
            100,
            1.0,
            {},
            {},
            QStringLiteral("Pixel size for rendered points.")
        },
    };
}

void VtkPointsExample::run(const QVariantMap& params, ExampleContext& context)
{
    context.clearScene();

    vtkNew<vtkPoints> points;
    points->InsertNextPoint(0, 0, 0);
    points->InsertNextPoint(1, 1, 1);
    points->InsertNextPoint(2, 2, 2);

    vtkNew<vtkPolyData> polydata;
    polydata->SetPoints(points);

    vtkNew<vtkVertexGlyphFilter> glyphFilter;
    glyphFilter->AddInputData(polydata);
    glyphFilter->Update();

    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputConnection(glyphFilter->GetOutputPort());

    m_pointsActor->SetMapper(mapper);
    m_pointsActor->GetProperty()->SetPointSize(params.value(QStringLiteral("size"), 12).toInt());
    m_pointsActor->GetProperty()->SetRenderPointsAsSpheres(true);
    m_pointsActor->GetProperty()->SetColor(1.0, 0.84, 0.0);

    auto* renderer = context.renderer();
    renderer->AddActor(m_pointsActor);
    renderer->SetBackground(0.0, 0.0, 0.0);
    renderer->ResetCamera();

    context.renderWindow()->Render();
    if (context.widget()) {
        context.widget()->update();
    }
}
