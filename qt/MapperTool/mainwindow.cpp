#include "mainwindow.h"
#include "aboutdialog.h"
#include "unitmapdialog.h"

#include "opencv/cv.h"

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

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(rotateOneStep()));
    timer->start(20);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::openMap( QString path )
{
    qDebug() << "Open map: " << path;
    fileName = path;
}

void MainWindow::saveMap( QString path )
{
    qDebug() << "Save map: " << path;
    fileName = path;
}



void MainWindow::fileRevert()
{
    if(fileName.isNull())
        fileOpen();
    else
        openMap(fileName);
}

void MainWindow::fileOpen()
{
    QString path = QFileDialog::getOpenFileName( this, tr("Open UV Map file"));
    if(path.isNull() == false)
    {
        openMap(path);
    }
}

void MainWindow::fileSave()
{
    if(fileName.isNull())
        fileSaveAs();
    else
        saveMap(fileName);
}

void MainWindow::fileSaveAs()
{
    QString path = QFileDialog::getSaveFileName( this, tr("Save UV Map as"));
    if(path.isNull() == false)
    {
        saveMap(path);
    }
}

void MainWindow::rotateOneStep()
{
    if (openGL)
        openGL->rotateBy(+2 * 16, +2 * 16, -1 * 16);
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


void MainWindow::showAboutDialog()
{
    AboutDialog *aboutDialog = new AboutDialog(this);
    aboutDialog->setWindowFlags(aboutDialog->windowFlags() ^ Qt::WindowContextHelpButtonHint);
    aboutDialog->exec();
}

void MainWindow::showUnitmapDialog()
{
    UnitmapDialog *unitmapDialog = new UnitmapDialog(this);
    unitmapDialog->setWindowFlags(unitmapDialog->windowFlags() ^ Qt::WindowContextHelpButtonHint);
    if(!unitmapDialog->exec())
        return;

    qDebug() << unitmapDialog->getSizeSetting();
}
