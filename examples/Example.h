#ifndef EXAMPLE_H
#define EXAMPLE_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QVariant>
#include <QVariantMap>
#include <memory>

class QVTKOpenGLNativeWidget;
class vtkGenericOpenGLRenderWindow;
class vtkRenderer;
class vtkRenderWindowInteractor;

enum class ExampleParameterType {
    Double,
    Integer,
    Boolean,
    Choice,
    FilePath
};

struct ExampleParameter {
    QString id;
    QString label;
    ExampleParameterType type = ExampleParameterType::Double;
    QVariant defaultValue;
    QVariant minimumValue;
    QVariant maximumValue;
    double singleStep = 1.0;
    QStringList options;
    QString placeholderText;
    QString description;
};

struct ExampleMetadata {
    QString id;
    QString title;
    QString categoryId;
    QString summary;
    QStringList tags;
    QString dataPath;
    QString thumbnailPath;
    QString documentationLink;
};

class ExampleContext
{
public:
    ExampleContext(vtkRenderer* renderer,
                   vtkGenericOpenGLRenderWindow* renderWindow,
                   QVTKOpenGLNativeWidget* widget,
                   QString dataRoot);

    vtkRenderer* renderer() const;
    vtkGenericOpenGLRenderWindow* renderWindow() const;
    QVTKOpenGLNativeWidget* widget() const;
    vtkRenderWindowInteractor* interactor() const;

    void clearScene(bool resetCamera = true);
    QString resolveDataPath(const QString& relativePath) const;

private:
    vtkRenderer* m_renderer = nullptr;
    vtkGenericOpenGLRenderWindow* m_renderWindow = nullptr;
    QVTKOpenGLNativeWidget* m_widget = nullptr;
    QString m_dataRoot;
};

class Example : public QObject
{
    Q_OBJECT
public:
    explicit Example(QObject* parent = nullptr);
    ~Example() override;

    virtual ExampleMetadata metadata() const = 0;
    virtual QList<ExampleParameter> parameters() const { return {}; }
    virtual void run(const QVariantMap& params, ExampleContext& context) = 0;
};

using ExamplePtr = std::unique_ptr<Example>;

#endif // EXAMPLE_H
