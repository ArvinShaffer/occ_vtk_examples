#ifndef VTKPRIMITIVESEXAMPLE_H
#define VTKPRIMITIVESEXAMPLE_H

#include "examples/Example.h"

#include <vtkSmartPointer.h>

class vtkActor;
class vtkPolyData;

class VtkPrimitivesExample : public Example
{
public:
    VtkPrimitivesExample();

    ExampleMetadata metadata() const override;
    QList<ExampleParameter> parameters() const override;
    void run(const QVariantMap& params, ExampleContext& context) override;

private:
    vtkSmartPointer<vtkActor> m_cylinderActor;
    vtkSmartPointer<vtkActor> m_cubeActor;
};

#endif // VTKPRIMITIVESEXAMPLE_H
