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
    
protected:
    void keyPressEvent(QKeyEvent* event);

private:
    QSettings settings;

    Ui::MainWindow *ui;
    GLWidget *glWidget;

    MapManager *uvMap;
    DisplayTextureManager *displayTexture;

    QString dataPath;

    QActionGroup *displayActionGroup;
    QActionGroup *transparencyGridActionGroup;

private slots:
    void initializeApp();

    void toggleFullscreen();
    void showAboutDialog();
    void showUnitmapDialog();
    void showPatternsDialog();
    void showInverseDialog();

    void fileRevert();
    void fileOpen();
    void fileSave();
    void fileSaveAs();

    void zoomIn();
    void zoomOut();
    void zoomReset();
    void zoomToFit();

    void selectDisplayTexture();
    void selectTransparencyGrid();
};

#endif // MAINWINDOW_H
