#!/usr/bin/env python3
"""
Scaffold a new OCC/VTK example class and register it inside the manifest.
"""

import argparse
import json
import os
from pathlib import Path
from textwrap import dedent


TEMPLATE_HEADER = """#ifndef {guard}
#define {guard}

#include "examples/Example.h"

#include <vtkSmartPointer.h>

class vtkActor;

class {class_name} : public Example
{{
public:
    ExampleMetadata metadata() const override;
    QList<ExampleParameter> parameters() const override;
    void run(const QVariantMap& params, ExampleContext& context) override;

private:
    vtkSmartPointer<vtkActor> m_actor;
}};

#endif // {guard}
"""

TEMPLATE_SOURCE = """#include "examples/{group}/{class_name}.h"

#include "examples/ExampleRegistry.h"
#include "global_headers.h"

namespace {{
ExampleRegistrar s_registrar(QStringLiteral("{class_name}"), []() {{
    return std::make_unique<{class_name}>();
}});
}}

ExampleMetadata {class_name}::metadata() const
{{
    ExampleMetadata meta;
    meta.id = QStringLiteral("{example_id}");
    meta.title = QStringLiteral("{title}");
    meta.categoryId = QStringLiteral("{category}");
    meta.summary = QStringLiteral("{description}");
    meta.tags = QStringList{{QStringLiteral("{tag}")}};
    return meta;
}}

QList<ExampleParameter> {class_name}::parameters() const
{{
    return {{
        ExampleParameter{{QStringLiteral("placeholder"),
                          QStringLiteral("Parameter"),
                          ExampleParameterType::Double,
                          1.0,
                          0.1,
                          10.0,
                          0.1,
                          {{}},
                          {{}},
                          QStringLiteral("Replace with a useful parameter.")}},
    }};
}}

void {class_name}::run(const QVariantMap& params, ExampleContext& context)
{{
    Q_UNUSED(params)
    context.clearScene();

    vtkNew<vtkSphereSource> sphere;
    sphere->SetThetaResolution(32);
    sphere->SetPhiResolution(32);

    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputConnection(sphere->GetOutputPort());

    m_actor = vtkSmartPointer<vtkActor>::New();
    m_actor->SetMapper(mapper);

    auto* renderer = context.renderer();
    renderer->AddActor(m_actor);
    renderer->ResetCamera();

    context.renderWindow()->Render();
    if (context.widget()) {{
        context.widget()->update();
    }}
}}
"""


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--class-name", required=True, help="C++ class name for the example.")
    parser.add_argument("--example-id", required=True, help="Unique identifier used in the manifest.")
    parser.add_argument("--category", required=True, help="Category id defined in the manifest.")
    parser.add_argument("--group", default="vtk", help="Subfolder under examples/ to place the files.")
    parser.add_argument("--title", default="New Example", help="Human readable title.")
    parser.add_argument("--description", default="Describe the example.", help="Short summary.")
    parser.add_argument("--tag", default="custom", help="Single default tag.")
    return parser.parse_args()


def ensure_manifest_entry(manifest_path: Path, example_entry: dict) -> None:
    manifest = {"categories": [], "examples": []}
    if manifest_path.exists():
        manifest = json.loads(manifest_path.read_text(encoding="utf-8"))

    examples = manifest.setdefault("examples", [])
    existing = next((ex for ex in examples if ex.get("id") == example_entry["id"]), None)
    if existing:
        existing.update(example_entry)
    else:
        examples.append(example_entry)

    manifest_path.write_text(json.dumps(manifest, indent=2, ensure_ascii=False), encoding="utf-8")


def main() -> None:
    args = parse_args()
    project_root = Path(__file__).resolve().parents[1]
    group_dir = project_root / "examples" / args.group
    group_dir.mkdir(parents=True, exist_ok=True)

    header_path = group_dir / f"{args.class_name}.h"
    source_path = group_dir / f"{args.class_name}.cpp"

    guard = f"{args.class_name.upper()}_H"
    header_content = TEMPLATE_HEADER.format(
        guard=guard,
        class_name=args.class_name,
    )
    source_content = TEMPLATE_SOURCE.format(
        group=args.group,
        class_name=args.class_name,
        example_id=args.example_id,
        title=args.title,
        category=args.category,
        description=args.description.replace('"', '\\"'),
        tag=args.tag,
    )

    header_path.write_text(dedent(header_content), encoding="utf-8")
    source_path.write_text(dedent(source_content), encoding="utf-8")

    manifest_entry = {
        "id": args.example_id,
        "class": args.class_name,
        "category": args.category,
        "title": args.title,
        "description": args.description,
        "tags": [args.tag],
    }
    ensure_manifest_entry(project_root / "data" / "examples_manifest.json", manifest_entry)

    print(f"Generated {header_path.relative_to(project_root)} and {source_path.relative_to(project_root)}.")


if __name__ == "__main__":
    main()
