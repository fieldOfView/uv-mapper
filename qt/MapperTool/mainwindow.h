#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QMainWindow>

#include "ui_mainwindow.h"

#include "glwidget.h"

#include "mapmanager.h"
#include "displaytexturemanager.h"

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

    MapManager map;
    DisplayTextureManager displayTexture;
    QString fileName;

private slots:
    void toggleFullscreen();
    void showAboutDialog();
    void showUnitmapDialog();

    void fileRevert();
    void fileOpen();
    void fileSave();
    void fileSaveAs();

    void selectDisplayTexture( );
};

#endif // MAINWINDOW_H
