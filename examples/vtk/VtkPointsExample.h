#ifndef VTKPOINTSEXAMPLE_H
#define VTKPOINTSEXAMPLE_H

#include "examples/Example.h"

#include <vtkSmartPointer.h>

class vtkActor;

class VtkPointsExample : public Example
{
public:
    VtkPointsExample();

    ExampleMetadata metadata() const override;
    QList<ExampleParameter> parameters() const override;
    void run(const QVariantMap& params, ExampleContext& context) override;

private:
    vtkSmartPointer<vtkActor> m_pointsActor;
};

#endif // VTKPOINTSEXAMPLE_H
