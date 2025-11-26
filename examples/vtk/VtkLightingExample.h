#ifndef VTKLIGHTINGEXAMPLE_H
#define VTKLIGHTINGEXAMPLE_H

#include "examples/Example.h"

#include <vtkSmartPointer.h>

class vtkActor;
class vtkLight;

class VtkLightingExample : public Example
{
public:
    VtkLightingExample();

    ExampleMetadata metadata() const override;
    QList<ExampleParameter> parameters() const override;
    void run(const QVariantMap& params, ExampleContext& context) override;

private:
    vtkSmartPointer<vtkActor> m_surfaceActor;
    vtkSmartPointer<vtkLight> m_light;
};

#endif // VTKLIGHTINGEXAMPLE_H
