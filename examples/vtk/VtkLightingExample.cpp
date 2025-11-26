#include "examples/vtk/VtkLightingExample.h"

#include "examples/ExampleRegistry.h"
#include "global_headers.h"

namespace {
ExampleRegistrar s_lightingReg(QStringLiteral("VtkLightingExample"), []() {
    return std::make_unique<VtkLightingExample>();
});
}

VtkLightingExample::VtkLightingExample()
{
    m_surfaceActor = vtkSmartPointer<vtkActor>::New();
    m_light = vtkSmartPointer<vtkLight>::New();
}

ExampleMetadata VtkLightingExample::metadata() const
{
    ExampleMetadata meta;
    meta.id = QStringLiteral("vtk_lighting");
    meta.title = QStringLiteral("Dynamic Lighting");
    meta.categoryId = QStringLiteral("vtk_rendering");
    meta.summary = QStringLiteral("Experiment with camera-aligned lights to highlight surfaces.");
    meta.tags = QStringList{QStringLiteral("vtk"), QStringLiteral("lighting")};
    return meta;
}

QList<ExampleParameter> VtkLightingExample::parameters() const
{
    return {
        ExampleParameter{
            QStringLiteral("intensity"),
            QStringLiteral("Light Intensity"),
            ExampleParameterType::Double,
            0.8,
            0.1,
            3.0,
            0.1,
            {},
            {},
            QStringLiteral("Overall power of the camera light.")
        },
        ExampleParameter{
            QStringLiteral("color"),
            QStringLiteral("Warm Light"),
            ExampleParameterType::Choice,
            QStringLiteral("Warm"),
            {},
            {},
            1.0,
            {QStringLiteral("Warm"), QStringLiteral("Cool"), QStringLiteral("Neutral")},
            {},
            QStringLiteral("Pick a preset color temperature.")
        },
    };
}

void VtkLightingExample::run(const QVariantMap& params, ExampleContext& context)
{
    context.clearScene();

    vtkNew<vtkSphereSource> surface;
    surface->SetThetaResolution(64);
    surface->SetPhiResolution(64);
    surface->SetRadius(5.0);

    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputConnection(surface->GetOutputPort());

    m_surfaceActor->SetMapper(mapper);
    m_surfaceActor->GetProperty()->SetColor(0.8, 0.8, 0.8);

    const double intensity = params.value(QStringLiteral("intensity"), 0.8).toDouble();
    const QString colorPreset = params.value(QStringLiteral("color"), QStringLiteral("Warm")).toString();

    if (colorPreset == QStringLiteral("Cool")) {
        m_light->SetColor(0.4, 0.6, 1.0);
    } else if (colorPreset == QStringLiteral("Neutral")) {
        m_light->SetColor(1.0, 1.0, 1.0);
    } else {
        m_light->SetColor(1.0, 0.85, 0.7);
    }

    m_light->SetLightTypeToCameraLight();
    m_light->SetIntensity(intensity);

    auto* renderer = context.renderer();
    renderer->AddActor(m_surfaceActor);
    renderer->AddLight(m_light);
    renderer->SetBackground(0.05, 0.05, 0.09);
    renderer->ResetCamera();

    context.renderWindow()->Render();
    if (context.widget()) {
        context.widget()->update();
    }
}
