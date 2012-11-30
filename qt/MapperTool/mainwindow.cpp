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

    // Connect any actions from the menubar to the root QWidget too,
    // so keyboard shortcuts work when the menuBar is hidden.
    addActions( menuBar()->actions() );

    openGL = new GLWidget( centralWidget() );
    setCentralWidget( openGL );

    //map = new MapManager();
    //displayTexture = new DisplayTextureManager();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::showUnitmapDialog()
{
    UnitmapDialog *unitmapDialog = new UnitmapDialog(this);
    unitmapDialog->setWindowFlags(unitmapDialog->windowFlags() ^ Qt::WindowContextHelpButtonHint);
    if(!unitmapDialog->exec())
        return;

    qDebug() << unitmapDialog->getSizeSetting();
}


void MainWindow::fileRevert()
{
    if(!map.load())
        fileOpen();
}

void MainWindow::fileOpen()
{
    QString path = QFileDialog::getOpenFileName( this, tr("Open UV Map file"));
    if(path.isNull() == false)
    {
        if(map.load(path)) {
            QRect rect = map.getRect();
            openGL->setAspectRatio((double)rect.width()/(double)rect.height());
            openGL->setTexture( map.getTexture() );
        }
    }
}

void MainWindow::fileSave()
{
    if(!map.save())
        fileSaveAs();
}

void MainWindow::fileSaveAs()
{
    QString path = QFileDialog::getSaveFileName( this, tr("Save UV Map as"));
    if(path.isNull() == false)
    {
        map.save(path);
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

void MainWindow::selectDisplayTexture( )
{

    QAction *action = (QAction *)sender();
    if( !action )
        return;

    QStringList actionNames;
    actionNames << "actionShowUV" << "actionShowU" << "actionShowV" << "actionShowAlpha" << "actionShowGrid" << "actionShowFile";

    DisplayTextureManager::DISPLAY_TYPE type = (DisplayTextureManager::DISPLAY_TYPE) actionNames.indexOf( action->objectName() );

    displayTexture.makeTexture( type );
}


void MainWindow::showAboutDialog()
{
    AboutDialog *aboutDialog = new AboutDialog(this);
    aboutDialog->setWindowFlags(aboutDialog->windowFlags() ^ Qt::WindowContextHelpButtonHint);
    aboutDialog->exec();
}
