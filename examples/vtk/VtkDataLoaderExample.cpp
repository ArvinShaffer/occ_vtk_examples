#include "examples/vtk/VtkDataLoaderExample.h"

#include "examples/ExampleRegistry.h"
#include "global_headers.h"

namespace {
ExampleRegistrar s_dataLoaderReg(QStringLiteral("VtkDataLoaderExample"), []() {
    return std::make_unique<VtkDataLoaderExample>();
});
}

VtkDataLoaderExample::VtkDataLoaderExample()
{
    m_actor = vtkSmartPointer<vtkActor>::New();
    m_grid = vtkSmartPointer<vtkUnstructuredGrid>::New();
}

ExampleMetadata VtkDataLoaderExample::metadata() const
{
    ExampleMetadata meta;
    meta.id = QStringLiteral("vtk_data_loader");
    meta.title = QStringLiteral("Unstructured Grid Loader");
    meta.categoryId = QStringLiteral("vtk_data");
    meta.summary = QStringLiteral("Load and display legacy VTK unstructured grid files.");
    meta.tags = QStringList{QStringLiteral("vtk"), QStringLiteral("io")};
    meta.dataPath = QStringLiteral("loadfile/test.vtk");
    return meta;
}

QList<ExampleParameter> VtkDataLoaderExample::parameters() const
{
    return {
        ExampleParameter{
            QStringLiteral("file"),
            QStringLiteral("VTK File"),
            ExampleParameterType::FilePath,
            metadata().dataPath,
            {},
            {},
            1.0,
            {},
            QStringLiteral("Select a legacy VTK file to load."),
            QStringLiteral("Legacy .vtk unstructured grid file path.")
        },
    };
}

void VtkDataLoaderExample::run(const QVariantMap& params, ExampleContext& context)
{
    context.clearScene();
    const auto fileParam = params.value(QStringLiteral("file"), metadata().dataPath).toString();
    const QString resolvedPath = context.resolveDataPath(fileParam);
    if (resolvedPath.isEmpty()) {
        return;
    }

    vtkNew<vtkUnstructuredGridReader> reader;
    reader->SetFileName(resolvedPath.toUtf8().constData());
    reader->Update();

    m_grid->DeepCopy(reader->GetOutput());

    vtkNew<vtkDataSetMapper> mapper;
    mapper->SetInputData(m_grid);

    m_actor->SetMapper(mapper);
    m_actor->GetProperty()->SetColor(0.9, 0.9, 0.9);
    m_actor->GetProperty()->EdgeVisibilityOff();

    auto* renderer = context.renderer();
    renderer->AddActor(m_actor);
    renderer->ResetCamera();
    context.renderWindow()->Render();
    if (context.widget()) {
        context.widget()->update();
    }
}
