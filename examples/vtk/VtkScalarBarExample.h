#ifndef VTKSCALARBAREXAMPLE_H
#define VTKSCALARBAREXAMPLE_H

#include "examples/Example.h"

#include <vtkSmartPointer.h>

class vtkActor;
class vtkLookupTable;
class vtkScalarBarActor;
class vtkScalarBarWidget;

class VtkScalarBarExample : public Example
{
public:
    VtkScalarBarExample();

    ExampleMetadata metadata() const override;
    QList<ExampleParameter> parameters() const override;
    void run(const QVariantMap& params, ExampleContext& context) override;

private:
    vtkSmartPointer<vtkActor> m_actor;
    vtkSmartPointer<vtkLookupTable> m_lookupTable;
    vtkSmartPointer<vtkScalarBarActor> m_scalarBarActor;
    vtkSmartPointer<vtkScalarBarWidget> m_scalarBarWidget;
};

#endif // VTKSCALARBAREXAMPLE_H
