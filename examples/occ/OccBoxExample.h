#ifndef OCCBOXEXAMPLE_H
#define OCCBOXEXAMPLE_H

#include "examples/Example.h"

#include <vtkSmartPointer.h>

class vtkActor;

class OccBoxExample : public Example
{
public:
    OccBoxExample();

    ExampleMetadata metadata() const override;
    QList<ExampleParameter> parameters() const override;
    void run(const QVariantMap& params, ExampleContext& context) override;

private:
    vtkSmartPointer<vtkActor> m_actor;
};

#endif // OCCBOXEXAMPLE_H
