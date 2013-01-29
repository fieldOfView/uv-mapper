#include "mainwindow.h"
#include "aboutdialog.h"
#include "unitmapdialog.h"

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <QDebug>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
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

    uvMap = new MapManager();
    displayTexture = new DisplayTextureManager();

    glWidget = new GLWidget( centralWidget() );
    connect(glWidget,SIGNAL(initialized()),this,SLOT(initializeApp()));
    setCentralWidget( glWidget );
}

MainWindow::~MainWindow()
{
    delete glWidget;
    delete uvMap;
    delete displayTexture;
    delete ui;
}

void MainWindow::initializeApp()
{
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


void MainWindow::fileRevert()
{
    if(!uvMap->load())
        fileOpen();
}

void MainWindow::fileOpen()
{
    QString path = QFileDialog::getOpenFileName( this, tr("Open UV Map file"));
    if(path.isNull() == false)
    {
        if(uvMap->load(path)) {
            glWidget->setMapTexture( uvMap->getTexture() );
        }
    }
}

void MainWindow::fileSave()
{
    if(!uvMap->save())
        fileSaveAs();
}

void MainWindow::fileSaveAs()
{
    QString path = QFileDialog::getSaveFileName( this, tr("Save UV Map as"));
    if(path.isNull() == false)
    {
        uvMap->save(path);
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
