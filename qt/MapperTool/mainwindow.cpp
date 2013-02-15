#include "mainwindow.h"
#include "aboutdialog.h"
#include "unitmapdialog.h"
#include "patternsdialog.h"
#include "inversedialog.h"

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <QDebug>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    dataPath = settings.value("DataPath").toString();

    ui->setupUi(this);

    displayActionGroup = new QActionGroup(this);
    displayActionGroup->addAction(ui->actionDisplayUV);
    displayActionGroup->addAction(ui->actionDisplayU);
    displayActionGroup->addAction(ui->actionDisplayV);
    displayActionGroup->addAction(ui->actionDisplayAlpha);
    displayActionGroup->addAction(ui->actionDisplayGrid);
    displayActionGroup->addAction(ui->actionDisplayFile);
    displayActionGroup->setExclusive(true);

    transparencyGridActionGroup = new QActionGroup(this);
    transparencyGridActionGroup->addAction(ui->actionGridNone);
    transparencyGridActionGroup->addAction(ui->actionGridLight);
    transparencyGridActionGroup->addAction(ui->actionGridDark);
    transparencyGridActionGroup->setExclusive(true);


    // Connect any actions from the menubar to the root QWidget too,
    // so keyboard shortcuts work when the menuBar is hidden.
    addActions( menuBar()->actions() );

    glWidget = new GLWidget( centralWidget() );
    connect(glWidget,SIGNAL(initialized()),this,SLOT(initializeApp()));
    setCentralWidget( glWidget );
}

MainWindow::~MainWindow()
{
    delete glWidget;
    delete uvMap;
    delete displayTexture;
    //delete ui;
}

void MainWindow::initializeApp()
{
    uvMap = new MapManager();
    displayTexture = new DisplayTextureManager();

    displayTexture->makeTexture(DisplayTextureManager::DISPLAY_TYPE_UV);
    glWidget->setDisplayTexture(displayTexture->getTexture());

    QRect screenSize = QApplication::desktop()->screenGeometry();
    GeneratedImage* newMap = new GeneratedImage(QSize(screenSize.width(), screenSize.height()), GL_RGBA16);
    newMap->drawGradient(QColor("#000000"), QColor("#ff0000"), QColor("#00ff00"), QColor("#ffff00"));
    uvMap->createFromTexture(newMap->getTexture());
    glWidget->setMapTexture(uvMap->getTexture());

    delete newMap;
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    if(event->key() == Qt::Key_Escape) {
        if(isFullScreen()) {
            showNormal();
            statusBar()->show();
            menuBar()->show();
        }
    }
}

void MainWindow::showUnitmapDialog()
{
    UnitmapDialog *unitmapDialog = new UnitmapDialog(this);
    unitmapDialog->setWindowFlags(unitmapDialog->windowFlags() ^ Qt::WindowContextHelpButtonHint);
    if(!unitmapDialog->exec())
        return;

    GeneratedImage* newMap = new GeneratedImage(unitmapDialog->getSizeSetting(), GL_RGBA16);
    newMap->drawGradient(QColor("#000000"), QColor("#ff0000"), QColor("#00ff00"), QColor("#ffff00"));
    uvMap->createFromTexture(newMap->getTexture());
    glWidget->setMapTexture(uvMap->getTexture());

    delete newMap;
}

void MainWindow::showPatternsDialog()
{
    PatternsDialog *patternsDialog = new PatternsDialog(this, glWidget);
    patternsDialog->setWindowFlags(patternsDialog->windowFlags() ^ Qt::WindowContextHelpButtonHint);
    patternsDialog->addAction(ui->actionFullscreen);
    patternsDialog->addAction(ui->actionZoomActualSize);
    patternsDialog->addAction(ui->actionZoomFit);
    patternsDialog->addAction(ui->actionZoomIn);
    patternsDialog->addAction(ui->actionZoomOut);

    int dialogResult = patternsDialog->exec();
    delete patternsDialog;

    if(!dialogResult)
        return;
}

void MainWindow::showInverseDialog()
{
    InverseDialog *inverseDialog = new InverseDialog(this);
    inverseDialog->setWindowFlags(inverseDialog->windowFlags() ^ Qt::WindowContextHelpButtonHint);
    if(!inverseDialog->exec())
        return;
}

void MainWindow::fileRevert()
{
    if(!uvMap->load(""))
        fileOpen();
}

void MainWindow::fileOpen()
{
    QString fileName = QFileDialog::getOpenFileName( this, tr("Open UV Map file"), dataPath, "UV Maps (*.png *.tif)" );
    if(fileName.isNull() == false)
    {
        dataPath = QFileInfo(fileName).path();
        settings.setValue("DataPath", dataPath);

        if(uvMap->load(fileName)) {
            glWidget->setMapTexture( uvMap->getTexture() );
        }
    }
}

void MainWindow::fileSave()
{
    if(!uvMap->save(""))
        fileSaveAs();
}

void MainWindow::fileSaveAs()
{
    QString fileName = QFileDialog::getSaveFileName( this, tr("Save UV Map as"), dataPath, "UV Maps (*.png *.tif)" );
    if(fileName.isNull() == false)
    {
        uvMap->save(fileName);
        dataPath = QFileInfo(fileName).path();
        settings.setValue("DataPath", dataPath);
    }
}

void MainWindow::toggleFullscreen()
{
    if( !isFullScreen() ) {
        showFullScreen();
        statusBar()->hide();
        menuBar()->hide();
    } else {
        showNormal();
        statusBar()->show();
        menuBar()->show();
    }
}

void MainWindow::zoomIn()
{
    glWidget->zoomInOut(true);
}

void MainWindow::zoomOut()
{
    glWidget->zoomInOut(false);
}

void MainWindow::zoomReset()
{
    glWidget->setZoom(1.0);
}

void MainWindow::zoomToFit()
{
    glWidget->setZoom(0.0);
}

void MainWindow::selectDisplayTexture()
{
    QAction *action = (QAction *)sender();
    if( !action )
        return;

    displayTexture->makeTexture((DisplayTextureManager::DISPLAY_TYPE)displayActionGroup->actions().indexOf(action));
    glWidget->setDisplayTexture(displayTexture->getTexture());
}

void MainWindow::selectTransparencyGrid()
{
    QAction *action = (QAction *)sender();
    if( !action )
        return;

    glWidget->setTransparencyGrid((GLWidget::TRANSPARENCYGRID_TYPE)transparencyGridActionGroup->actions().indexOf(action));
}

void MainWindow::showAboutDialog()
{
    AboutDialog *aboutDialog = new AboutDialog(this);
    aboutDialog->setWindowFlags(aboutDialog->windowFlags() ^ Qt::WindowContextHelpButtonHint);
    aboutDialog->exec();
}
