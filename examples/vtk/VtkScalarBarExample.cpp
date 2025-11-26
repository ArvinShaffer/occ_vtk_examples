#include "examples/vtk/VtkScalarBarExample.h"

#include "examples/ExampleRegistry.h"
#include "global_headers.h"

namespace {
ExampleRegistrar s_scalarBarReg(QStringLiteral("VtkScalarBarExample"), []() {
    return std::make_unique<VtkScalarBarExample>();
});
}

VtkScalarBarExample::VtkScalarBarExample()
{
    m_actor = vtkSmartPointer<vtkActor>::New();
    m_lookupTable = vtkSmartPointer<vtkLookupTable>::New();
    m_scalarBarActor = vtkSmartPointer<vtkScalarBarActor>::New();
    m_scalarBarWidget = vtkSmartPointer<vtkScalarBarWidget>::New();
}

ExampleMetadata VtkScalarBarExample::metadata() const
{
    ExampleMetadata meta;
    meta.id = QStringLiteral("vtk_scalar_bar");
    meta.title = QStringLiteral("Scalar Bar Overlay");
    meta.categoryId = QStringLiteral("vtk_rendering");
    meta.summary = QStringLiteral("Attach a scalar bar widget to the Qt interactor.");
    meta.tags = QStringList{QStringLiteral("vtk"), QStringLiteral("annotation")};
    meta.dataPath = QStringLiteral("Ex06.vtk");
    return meta;
}

QList<ExampleParameter> VtkScalarBarExample::parameters() const
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
            QStringLiteral("Unstructured grid with scalar data.")
        },
        ExampleParameter{
            QStringLiteral("horizontal"),
            QStringLiteral("Horizontal Bar"),
            ExampleParameterType::Boolean,
            true,
            {},
            {},
            1.0,
            {},
            {},
            QStringLiteral("Display the bar horizontally.")
        },
    };
}

void VtkScalarBarExample::run(const QVariantMap& params, ExampleContext& context)
{
    context.clearScene();

    const QString datasetParam = params.value(QStringLiteral("dataset"), metadata().dataPath).toString();
    const QString datasetPath = context.resolveDataPath(datasetParam);
    if (datasetPath.isEmpty()) {
        return;
    }

    vtkNew<vtkUnstructuredGridReader> reader;
    reader->SetFileName(datasetPath.toUtf8().constData());
    reader->Update();

    m_lookupTable->Build();

    vtkNew<vtkDataSetMapper> mapper;
    mapper->SetInputData(reader->GetOutput());
    mapper->SetScalarRange(reader->GetOutput()->GetScalarRange());
    mapper->SetLookupTable(m_lookupTable);

    m_actor->SetMapper(mapper);

    auto* renderer = context.renderer();
    renderer->AddActor(m_actor);
    renderer->ResetCamera();

    const bool horizontal = params.value(QStringLiteral("horizontal"), true).toBool();
    if (m_scalarBarWidget && context.interactor()) {
        m_scalarBarActor->SetLookupTable(m_lookupTable);
        if (horizontal) {
            m_scalarBarActor->SetOrientationToHorizontal();
        } else {
            m_scalarBarActor->SetOrientationToVertical();
        }
        m_scalarBarActor->DrawBackgroundOn();
        m_scalarBarActor->GetBackgroundProperty()->SetColor(0, 0, 0);

        m_scalarBarWidget->SetInteractor(context.interactor());
        m_scalarBarWidget->SetScalarBarActor(m_scalarBarActor);
        m_scalarBarWidget->EnabledOn();
        m_scalarBarWidget->RepositionableOn();
    }

    context.renderWindow()->Render();
    if (context.widget()) {
        context.widget()->update();
    }
}
