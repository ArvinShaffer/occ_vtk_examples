#include "examples/vtk/VtkVolumeRenderingExample.h"

#include "examples/ExampleRegistry.h"
#include "global_headers.h"

namespace {
ExampleRegistrar s_volumeReg(QStringLiteral("VtkVolumeRenderingExample"), []() {
    return std::make_unique<VtkVolumeRenderingExample>();
});
}

VtkVolumeRenderingExample::VtkVolumeRenderingExample()
{
    m_volume = vtkSmartPointer<vtkVolume>::New();
}

ExampleMetadata VtkVolumeRenderingExample::metadata() const
{
    ExampleMetadata meta;
    meta.id = QStringLiteral("vtk_volume");
    meta.title = QStringLiteral("Volume Ray Casting");
    meta.categoryId = QStringLiteral("vtk_rendering");
    meta.summary = QStringLiteral("Fixed-point ray casting of structured points data.");
    meta.tags = QStringList{QStringLiteral("vtk"), QStringLiteral("volume")};
    meta.dataPath = QStringLiteral("Ex04.vtk");
    return meta;
}

QList<ExampleParameter> VtkVolumeRenderingExample::parameters() const
{
    return {
        ExampleParameter{
            QStringLiteral("dataset"),
            QStringLiteral("Dataset"),
            ExampleParameterType::FilePath,
            metadata().dataPath,
            {},
            {},
            1.0,
            {},
            {},
            QStringLiteral("Structured points volume source file.")
        },
        ExampleParameter{
            QStringLiteral("shade"),
            QStringLiteral("Enable Shading"),
            ExampleParameterType::Boolean,
            true,
            {},
            {},
            1.0,
            {},
            {},
            QStringLiteral("Toggle shading on the volume.")
        },
        ExampleParameter{
            QStringLiteral("sampleDistance"),
            QStringLiteral("Sample Distance"),
            ExampleParameterType::Double,
            1.0,
            0.1,
            5.0,
            0.1,
            {},
            {},
            QStringLiteral("Adjust ray sample distance.")
        },
    };
}

void VtkVolumeRenderingExample::run(const QVariantMap& params, ExampleContext& context)
{
    context.clearScene();

    const QString datasetParam = params.value(QStringLiteral("dataset"), metadata().dataPath).toString();
    const QString datasetPath = context.resolveDataPath(datasetParam);
    if (datasetPath.isEmpty()) {
        return;
    }

    vtkNew<vtkStructuredPointsReader> reader;
    reader->SetFileName(datasetPath.toUtf8().constData());
    reader->Update();

    vtkNew<vtkFixedPointVolumeRayCastMapper> mapper;
    mapper->SetInputConnection(reader->GetOutputPort());
    mapper->SetSampleDistance(params.value(QStringLiteral("sampleDistance"), 1.0).toDouble());

    vtkNew<vtkVolumeProperty> volumeProperty;
    volumeProperty->SetInterpolationTypeToLinear();
    const bool shade = params.value(QStringLiteral("shade"), true).toBool();
    volumeProperty->SetShade(shade);
    volumeProperty->SetAmbient(0.4);
    volumeProperty->SetDiffuse(0.6);
    volumeProperty->SetSpecular(0.2);

    vtkNew<vtkPiecewiseFunction> opacity;
    opacity->AddPoint(70, 0.00);
    opacity->AddPoint(90, 0.40);
    opacity->AddPoint(180, 0.60);
    volumeProperty->SetScalarOpacity(opacity);

    vtkNew<vtkColorTransferFunction> color;
    color->AddRGBPoint(0.000, 0.00, 0.00, 0.00);
    color->AddRGBPoint(64.00, 1.00, 0.52, 0.30);
    color->AddRGBPoint(190.0, 1.00, 1.00, 1.00);
    color->AddRGBPoint(220.0, 0.20, 0.20, 0.20);
    volumeProperty->SetColor(color);

    m_volume->SetMapper(mapper);
    m_volume->SetProperty(volumeProperty);

    auto* renderer = context.renderer();
    renderer->AddVolume(m_volume);
    renderer->SetBackground(0, 0, 0);
    renderer->ResetCamera();

    context.renderWindow()->Render();
    if (context.widget()) {
        context.widget()->update();
    }
}
