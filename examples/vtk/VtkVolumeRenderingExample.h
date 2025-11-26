#ifndef VTKVOLUMERENDERINGEXAMPLE_H
#define VTKVOLUMERENDERINGEXAMPLE_H

#include "examples/Example.h"

#include <vtkSmartPointer.h>

class vtkVolume;

class VtkVolumeRenderingExample : public Example
{
public:
    VtkVolumeRenderingExample();

    ExampleMetadata metadata() const override;
    QList<ExampleParameter> parameters() const override;
    void run(const QVariantMap& params, ExampleContext& context) override;

private:
    vtkSmartPointer<vtkVolume> m_volume;
};

#endif // VTKVOLUMERENDERINGEXAMPLE_H
