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
    QSettings m_settings;

    Ui::MainWindow *m_ui;
    GLWidget *m_glWidget;

    MapManager *m_uvMap;
    DisplayTextureManager *m_displayTexture;

    QString m_dataPath;

    QActionGroup *m_displayActionGroup;
    QActionGroup *m_transparencyGridActionGroup;

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

    void editUndo();
    void editRedo();

    void filterGaussian();
    void filterMedian();
    void filterDespeckle();
    void filterFillHoles();

    void zoomIn();
    void zoomOut();
    void zoomReset();
    void zoomToFit();

    void selectDisplayTexture();
    void selectTransparencyGrid();
};

#endif // MAINWINDOW_H
