#ifndef VTKLOOKUPTABLEEXAMPLE_H
#define VTKLOOKUPTABLEEXAMPLE_H

#include "examples/Example.h"

#include <vtkSmartPointer.h>

class vtkActor;
class vtkUnstructuredGrid;

class VtkLookupTableExample : public Example
{
public:
    VtkLookupTableExample();

    ExampleMetadata metadata() const override;
    void run(const QVariantMap& params, ExampleContext& context) override;

private:
    vtkSmartPointer<vtkActor> m_actor;
    vtkSmartPointer<vtkUnstructuredGrid> m_grid;
};

#endif // VTKLOOKUPTABLEEXAMPLE_H
