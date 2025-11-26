#ifndef VTKDATALOADEREXAMPLE_H
#define VTKDATALOADEREXAMPLE_H

#include "examples/Example.h"

#include <vtkSmartPointer.h>

class vtkActor;
class vtkUnstructuredGrid;

class VtkDataLoaderExample : public Example
{
public:
    VtkDataLoaderExample();

    ExampleMetadata metadata() const override;
    QList<ExampleParameter> parameters() const override;
    void run(const QVariantMap& params, ExampleContext& context) override;

private:
    vtkSmartPointer<vtkActor> m_actor;
    vtkSmartPointer<vtkUnstructuredGrid> m_grid;
};

#endif // VTKDATALOADEREXAMPLE_H
