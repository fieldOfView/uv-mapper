#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QMainWindow>

#include "ui_mainwindow.h"

#include "glwidget.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    

private:
    Ui::MainWindow *ui;

    GLWidget *openGL;

private slots:
    void rotateOneStep();

    void toggleFullscreen();
    void showAboutDialog();
    void showUnitmapDialog();
};

#endif // MAINWINDOW_H
