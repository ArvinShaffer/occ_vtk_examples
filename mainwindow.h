#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMap>
#include <memory>

#include "examples/Example.h"
#include "examples/ExampleRegistry.h"
#include "global_headers.h"

namespace Ui {
class MainWindow;
}

class QFormLayout;
class QListWidgetItem;
class QTreeWidgetItem;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    vtkSmartPointer<vtkRenderer> main_render_3d;
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renWin3d;
    vtkSmartPointer<vtkNamedColors> colors;

private:
    Ui::MainWindow *ui;
    ExampleRegistry& m_registry;
    ExampleDefinition m_currentDefinition;
    ExamplePtr m_activeExample;
    QList<ExampleParameter> m_currentParameters;
    QMap<QString, QWidget*> m_parameterEditors;
    QFormLayout* m_parametersLayout = nullptr;
    QString m_currentCategoryId;
    QString m_currentExampleId;
    QString m_dataRoot;

    void initializeRenderWindow();
    void initializeNavigation();
    void loadManifest();
    void populateCategoryTree();
    void populateExamplesForCategory(const QString& categoryId);
    void applySearchFilter(const QString& pattern);
    void displayExample(const QString& exampleId);
    void rebuildParameterPanel();
    QWidget* createEditor(const ExampleParameter& parameter);
    QVariant editorValue(const ExampleParameter& parameter, QWidget* editor) const;
    QVariantMap gatherParameters() const;
    QString resolveDataPath(const QString& relative) const;
    QString locateDataDirectory() const;
    void runActiveExample();
    void appendLog(const QString& message);

private slots:
    void handleCategorySelection();
    void handleExampleSelection();
    void handleExampleActivation(QListWidgetItem* item);
    void handleRunExample();
    void handleSearchTextChanged(const QString& text);
};

#endif // MAINWINDOW_H
