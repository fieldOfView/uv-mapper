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

    QString fileName;

    void openMap( QString path );
    void saveMap( QString path );

private slots:
    void rotateOneStep();

    void toggleFullscreen();
    void showAboutDialog();
    void showUnitmapDialog();

    void fileRevert();
    void fileOpen();
    void fileSave();
    void fileSaveAs();
};

#endif // MAINWINDOW_H
