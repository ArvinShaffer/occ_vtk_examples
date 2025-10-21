#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include "global_headers.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //Render
    vtkSmartPointer<vtkRenderer> main_render_3d;
    //renWin
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renWin3d;
    //actor
    vtkSmartPointer<vtkActor> current_actor;
    vtkSmartPointer<vtkUnstructuredGrid> ugrid;

    //colors
    vtkSmartPointer<vtkNamedColors> colors;

private slots:
    void on_btn_load_clicked();

    void on_btn_occbox_clicked();

    void on_ex01_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
