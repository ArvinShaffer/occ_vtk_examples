#include "examples/occ/OccBoxExample.h"

#include "examples/ExampleRegistry.h"
#include "global_headers.h"

namespace {
ExampleRegistrar s_registrar(QStringLiteral("OccBoxExample"), []() {
    return std::make_unique<OccBoxExample>();
});
}

OccBoxExample::OccBoxExample()
{
    m_actor = vtkSmartPointer<vtkActor>::New();
}

ExampleMetadata OccBoxExample::metadata() const
{
    ExampleMetadata meta;
    meta.id = QStringLiteral("occ_box");
    meta.title = QStringLiteral("OpenCascade Box");
    meta.categoryId = QStringLiteral("occ_shapes");
    meta.summary = QStringLiteral("Create a parametric box using OpenCascade "
                                  "and render it through the OCC-VTK bridge.");
    meta.tags = QStringList{QStringLiteral("occ"), QStringLiteral("primitives")};
    return meta;
}

QList<ExampleParameter> OccBoxExample::parameters() const
{
    return {
        ExampleParameter{
            QStringLiteral("length"),
            QStringLiteral("Length"),
            ExampleParameterType::Double,
            1.0,
            0.1,
            10.0,
            0.1,
            {},
            {},
            QStringLiteral("Length of the box along the X axis.")
        },
        ExampleParameter{
            QStringLiteral("width"),
            QStringLiteral("Width"),
            ExampleParameterType::Double,
            2.0,
            0.1,
            10.0,
            0.1,
            {},
            {},
            QStringLiteral("Width of the box along the Y axis.")
        },
        ExampleParameter{
            QStringLiteral("height"),
            QStringLiteral("Height"),
            ExampleParameterType::Double,
            3.0,
            0.1,
            10.0,
            0.1,
            {},
            {},
            QStringLiteral("Height of the box along the Z axis.")
        },
    };
}

void OccBoxExample::run(const QVariantMap& params, ExampleContext& context)
{
    context.clearScene();
    const double length = params.value(QStringLiteral("length"), 1.0).toDouble();
    const double width = params.value(QStringLiteral("width"), 2.0).toDouble();
    const double height = params.value(QStringLiteral("height"), 3.0).toDouble();

    BRepPrimAPI_MakeBox mkBox(length, width, height);
    const TopoDS_Shape& shape = mkBox.Shape();

    vtkNew<IVtkTools_ShapeDataSource> occSource;
    occSource->SetShape(new IVtkOCC_Shape(shape));

    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputConnection(occSource->GetOutputPort());

    m_actor->SetMapper(mapper);
    m_actor->GetProperty()->SetColor(0.86, 0.27, 0.21);
    m_actor->GetProperty()->SetOpacity(0.95);

    auto* renderer = context.renderer();
    renderer->AddActor(m_actor);
    renderer->SetBackground(0.1, 0.12, 0.16);
    renderer->ResetCamera();
    context.renderWindow()->Render();
    if (context.widget()) {
        context.widget()->update();
    }
}
