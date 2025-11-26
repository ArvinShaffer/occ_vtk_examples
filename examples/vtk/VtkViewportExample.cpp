#include "examples/vtk/VtkViewportExample.h"

#include "examples/ExampleRegistry.h"
#include "global_headers.h"

namespace {
ExampleRegistrar s_viewportReg(QStringLiteral("VtkViewportExample"), []() {
    return std::make_unique<VtkViewportExample>();
});
}

VtkViewportExample::VtkViewportExample()
{
    m_axesWidget = vtkSmartPointer<vtkOrientationMarkerWidget>::New();
}

ExampleMetadata VtkViewportExample::metadata() const
{
    ExampleMetadata meta;
    meta.id = QStringLiteral("vtk_viewports");
    meta.title = QStringLiteral("Multi-Viewport Layout");
    meta.categoryId = QStringLiteral("vtk_rendering");
    meta.summary = QStringLiteral("Compose four independent renderers inside the same window.");
    meta.tags = QStringList{QStringLiteral("vtk"), QStringLiteral("viewport")};
    return meta;
}

QList<ExampleParameter> VtkViewportExample::parameters() const
{
    return {
        ExampleParameter{
            QStringLiteral("showAxes"),
            QStringLiteral("Show Axes"),
            ExampleParameterType::Boolean,
            true,
            {},
            {},
            1.0,
            {},
            {},
            QStringLiteral("Display an orientation marker in the lower-left corner.")
        },
    };
}

void VtkViewportExample::run(const QVariantMap& params, ExampleContext& context)
{
    context.clearScene(false);

    vtkNew<vtkConeSource> cone;
    vtkNew<vtkCubeSource> cube;
    vtkNew<vtkCylinderSource> cylinder;
    vtkNew<vtkSphereSource> sphere;

    vtkNew<vtkPolyDataMapper> coneMapper;
    coneMapper->SetInputConnection(cone->GetOutputPort());
    vtkNew<vtkPolyDataMapper> cubeMapper;
    cubeMapper->SetInputConnection(cube->GetOutputPort());
    vtkNew<vtkPolyDataMapper> cylinderMapper;
    cylinderMapper->SetInputConnection(cylinder->GetOutputPort());
    vtkNew<vtkPolyDataMapper> sphereMapper;
    sphereMapper->SetInputConnection(sphere->GetOutputPort());

    vtkNew<vtkActor> coneActor;
    coneActor->SetMapper(coneMapper);
    vtkNew<vtkActor> cubeActor;
    cubeActor->SetMapper(cubeMapper);
    vtkNew<vtkActor> cylinderActor;
    cylinderActor->SetMapper(cylinderMapper);
    vtkNew<vtkActor> sphereActor;
    sphereActor->SetMapper(sphereMapper);

    auto* primary = context.renderer();
    primary->AddActor(coneActor);
    primary->SetBackground(1.0, 0.0, 0.0);
    primary->SetViewport(0.0, 0.0, 0.5, 0.5);

    vtkNew<vtkRenderer> r2;
    r2->AddActor(cubeActor);
    r2->SetBackground(0.0, 1.0, 0.0);
    r2->SetViewport(0.5, 0.0, 1.0, 0.5);

    vtkNew<vtkRenderer> r3;
    r3->AddActor(cylinderActor);
    r3->SetBackground(0.0, 0.0, 1.0);
    r3->SetViewport(0.0, 0.5, 0.5, 1.0);

    vtkNew<vtkRenderer> r4;
    r4->AddActor(sphereActor);
    r4->SetBackground(1.0, 1.0, 0.0);
    r4->SetViewport(0.5, 0.5, 1.0, 1.0);

    auto* renderWindow = context.renderWindow();
    renderWindow->AddRenderer(r2);
    renderWindow->AddRenderer(r3);
    renderWindow->AddRenderer(r4);

    const bool showAxes = params.value(QStringLiteral("showAxes"), true).toBool();
    if (showAxes && context.interactor()) {
        vtkNew<vtkAxesActor> axesActor;
        m_axesWidget->SetViewport(0.0, 0.0, 0.2, 0.2);
        m_axesWidget->SetOrientationMarker(axesActor);
        m_axesWidget->SetInteractor(context.interactor());
        m_axesWidget->EnabledOn();
        m_axesWidget->InteractiveOff();
    } else if (!showAxes) {
        m_axesWidget->EnabledOff();
    }

    renderWindow->Render();
    if (context.widget()) {
        context.widget()->update();
    }
}
