#include "examples/vtk/VtkPrimitivesExample.h"

#include "examples/ExampleRegistry.h"
#include "global_headers.h"

namespace {
ExampleRegistrar s_primitivesReg(QStringLiteral("VtkPrimitivesExample"), []() {
    return std::make_unique<VtkPrimitivesExample>();
});
}

VtkPrimitivesExample::VtkPrimitivesExample()
{
    m_cylinderActor = vtkSmartPointer<vtkActor>::New();
    m_cubeActor = vtkSmartPointer<vtkActor>::New();
}

ExampleMetadata VtkPrimitivesExample::metadata() const
{
    ExampleMetadata meta;
    meta.id = QStringLiteral("vtk_primitives");
    meta.title = QStringLiteral("Cylinder & Cube Composition");
    meta.categoryId = QStringLiteral("vtk_basics");
    meta.summary = QStringLiteral("Combine multiple primitive sources to explore "
                                  "basic geometry pipelines.");
    meta.tags = QStringList{QStringLiteral("vtk"), QStringLiteral("geometry")};
    return meta;
}

QList<ExampleParameter> VtkPrimitivesExample::parameters() const
{
    return {
        ExampleParameter{
            QStringLiteral("opacity"),
            QStringLiteral("Cylinder Opacity"),
            ExampleParameterType::Double,
            0.35,
            0.05,
            1.0,
            0.05,
            {},
            {},
            QStringLiteral("Transparency applied to the cylinder actor.")
        },
        ExampleParameter{
            QStringLiteral("resolution"),
            QStringLiteral("Cylinder Resolution"),
            ExampleParameterType::Integer,
            64,
            8,
            256,
            1.0,
            {},
            {},
            QStringLiteral("Number of segments for the cylinder.")
        },
    };
}

void VtkPrimitivesExample::run(const QVariantMap& params, ExampleContext& context)
{
    context.clearScene();

    const double opacity = params.value(QStringLiteral("opacity"), 0.35).toDouble();
    const int resolution = params.value(QStringLiteral("resolution"), 64).toInt();

    vtkNew<vtkCylinderSource> cylinder;
    cylinder->SetCenter(0, 0, 0);
    cylinder->SetHeight(8.0);
    cylinder->SetRadius(3.0);
    cylinder->SetResolution(resolution);
    cylinder->Update();

    vtkNew<vtkCubeSource> cube;
    cube->SetCenter(5, 5, 5);

    vtkNew<vtkPolyDataMapper> cylinderMapper;
    cylinderMapper->SetInputConnection(cylinder->GetOutputPort());

    vtkNew<vtkPolyDataMapper> cubeMapper;
    cubeMapper->SetInputConnection(cube->GetOutputPort());

    m_cylinderActor->SetMapper(cylinderMapper);
    m_cylinderActor->GetProperty()->SetOpacity(opacity);
    m_cubeActor->SetMapper(cubeMapper);
    m_cubeActor->GetProperty()->SetColor(0.85, 0.65, 0.13);

    auto* renderer = context.renderer();
    renderer->AddActor(m_cylinderActor);
    renderer->AddActor(m_cubeActor);
    renderer->ResetCamera();
    context.renderWindow()->Render();
    if (context.widget()) {
        context.widget()->update();
    }
}
