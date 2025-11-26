#ifndef VTKVIEWPORTEXAMPLE_H
#define VTKVIEWPORTEXAMPLE_H

#include "examples/Example.h"

#include <vtkSmartPointer.h>

class vtkOrientationMarkerWidget;

class VtkViewportExample : public Example
{
public:
    VtkViewportExample();

    ExampleMetadata metadata() const override;
    QList<ExampleParameter> parameters() const override;
    void run(const QVariantMap& params, ExampleContext& context) override;

private:
    vtkSmartPointer<vtkOrientationMarkerWidget> m_axesWidget;
};

#endif // VTKVIEWPORTEXAMPLE_H
