#include "examples/Example.h"

#include <QDir>
#include <QFileInfo>

#include <QVTKOpenGLNativeWidget.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkLightCollection.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>

Example::Example(QObject* parent)
    : QObject(parent)
{
}

Example::~Example() = default;

ExampleContext::ExampleContext(vtkRenderer* renderer,
                               vtkGenericOpenGLRenderWindow* renderWindow,
                               QVTKOpenGLNativeWidget* widget,
                               QString dataRoot)
    : m_renderer(renderer)
    , m_renderWindow(renderWindow)
    , m_widget(widget)
    , m_dataRoot(std::move(dataRoot))
{
}

vtkRenderer* ExampleContext::renderer() const
{
    return m_renderer;
}

vtkGenericOpenGLRenderWindow* ExampleContext::renderWindow() const
{
    return m_renderWindow;
}

QVTKOpenGLNativeWidget* ExampleContext::widget() const
{
    return m_widget;
}

vtkRenderWindowInteractor* ExampleContext::interactor() const
{
    if (!m_renderWindow) {
        return nullptr;
    }
    return m_renderWindow->GetInteractor();
}

void ExampleContext::clearScene(bool resetCamera)
{
    if (!m_renderer || !m_renderWindow) {
        return;
    }

    if (auto* collection = m_renderWindow->GetRenderers()) {
        collection->RemoveAllItems();
    }
    m_renderWindow->AddRenderer(m_renderer);
    m_renderer->RemoveAllViewProps();
    m_renderer->GetLights()->RemoveAllItems();
    if (resetCamera) {
        m_renderer->ResetCamera();
    }
    m_renderWindow->Render();
    if (m_widget) {
        m_widget->update();
    }
}

QString ExampleContext::resolveDataPath(const QString& relativePath) const
{
    if (relativePath.isEmpty()) {
        return {};
    }
    QFileInfo info(relativePath);
    if (info.isAbsolute() || relativePath.startsWith(":/")) {
        return relativePath;
    }
    QDir rootDir(m_dataRoot);
    return rootDir.filePath(relativePath);
}
