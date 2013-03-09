#include "mainwindow.h"
#include "aboutdialog.h"
#include "unitmapdialog.h"
#include "patternsdialog.h"
#include "inversedialog.h"

#include "mapoperations.h"

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <QDebug>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow)
{
    m_dataPath = m_settings.value("DataPath").toString();

    m_ui->setupUi(this);

    m_displayActionGroup = new QActionGroup(this);
    m_displayActionGroup->addAction(m_ui->actionDisplayUV);
    m_displayActionGroup->addAction(m_ui->actionDisplayU);
    m_displayActionGroup->addAction(m_ui->actionDisplayV);
    m_displayActionGroup->addAction(m_ui->actionDisplayAlpha);
    m_displayActionGroup->addAction(m_ui->actionDisplayGrid);
    m_displayActionGroup->addAction(m_ui->actionDisplayFile);
    m_displayActionGroup->setExclusive(true);

    m_transparencyGridActionGroup = new QActionGroup(this);
    m_transparencyGridActionGroup->addAction(m_ui->actionGridNone);
    m_transparencyGridActionGroup->addAction(m_ui->actionGridLight);
    m_transparencyGridActionGroup->addAction(m_ui->actionGridDark);
    m_transparencyGridActionGroup->setExclusive(true);


    // Connect any actions from the menubar to the root QWidget too,
    // so keyboard shortcuts work when the menuBar is hidden.
    addActions( menuBar()->actions() );

    m_glWidget = new GLWidget( centralWidget() );
    connect(m_glWidget,SIGNAL(initialized()),this,SLOT(initializeApp()));
    setCentralWidget( m_glWidget );
}

MainWindow::~MainWindow()
{
    delete m_glWidget;
    delete m_uvMap;
    delete m_displayTexture;
    delete m_ui;
}

void MainWindow::initializeApp()
{
    m_uvMap = new MapManager();
    m_displayTexture = new DisplayTextureManager();

    m_displayTexture->makeTexture(DisplayTextureManager::DISPLAY_TYPE_UV);
    m_glWidget->setDisplayTexture(m_displayTexture->getTexture());

    QRect screenSize = QApplication::desktop()->screenGeometry();
    GeneratedImage* newMap = new GeneratedImage(QSize(screenSize.width(), screenSize.height()), GL_RGBA16);
    newMap->drawGradient(QColor("#000000"), QColor("#ff0000"), QColor("#00ff00"), QColor("#ffff00"));
    m_uvMap->createFromTexture(newMap->getTexture());
    m_glWidget->setMapTexture(m_uvMap->getTexture());

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
    m_uvMap->createFromTexture(newMap->getTexture());
    m_glWidget->setMapTexture(m_uvMap->getTexture());

    delete newMap;
}

void MainWindow::showPatternsDialog()
{
    PatternsDialog *patternsDialog = new PatternsDialog(this, m_glWidget);
    patternsDialog->setWindowFlags(patternsDialog->windowFlags() ^ Qt::WindowContextHelpButtonHint);
    patternsDialog->addAction(m_ui->actionFullscreen);
    patternsDialog->addAction(m_ui->actionZoomActualSize);
    patternsDialog->addAction(m_ui->actionZoomFit);
    patternsDialog->addAction(m_ui->actionZoomIn);
    patternsDialog->addAction(m_ui->actionZoomOut);

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

    MapOperations *mapOperation = new MapOperations(m_uvMap->getMat());
    m_uvMap->setMat(mapOperation->inverse(inverseDialog->getSizeSetting(), inverseDialog->getDepthSetting(), inverseDialog->getCenteredSetting()));
    m_glWidget->repaint();

    delete mapOperation;
    delete inverseDialog;
}

void MainWindow::fileRevert()
{
    if(!m_uvMap->load(""))
        fileOpen();

    m_glWidget->repaint();
}

void MainWindow::fileOpen()
{
    QString fileName = QFileDialog::getOpenFileName( this, tr("Open UV Map file"), m_dataPath, "UV Maps (*.png *.tif)" );
    if(fileName.isNull() == false)
    {
        m_dataPath = QFileInfo(fileName).path();
        m_settings.setValue("DataPath", m_dataPath);

        if(m_uvMap->load(fileName)) {
            m_glWidget->setMapTexture( m_uvMap->getTexture() );
        }
    }
}

void MainWindow::fileSave()
{
    if(!m_uvMap->save(""))
        fileSaveAs();
}

void MainWindow::fileSaveAs()
{
    QString fileName = QFileDialog::getSaveFileName( this, tr("Save UV Map as"), m_dataPath, "UV Maps (*.png *.tif)" );
    if(fileName.isNull() == false)
    {
        m_uvMap->save(fileName);
        m_dataPath = QFileInfo(fileName).path();
        m_settings.setValue("DataPath", m_dataPath);
    }
}

void MainWindow::editUndo()
{
    m_uvMap->undo();
    m_glWidget->repaint();
}

void MainWindow::editRedo()
{
    m_uvMap->redo();
    m_glWidget->repaint();
}

void MainWindow::filterGaussian()
{
    MapOperations *mapOperation = new MapOperations(m_uvMap->getMat());
    m_uvMap->setMat(mapOperation->guassianBlur(1.0));

    delete mapOperation;

    m_glWidget->repaint();
}

void MainWindow::filterMedian()
{
    MapOperations *mapOperation = new MapOperations(m_uvMap->getMat());
    m_uvMap->setMat(mapOperation->medianBlur(3.0));

    delete mapOperation;

    m_glWidget->repaint();
}

void MainWindow::filterDespeckle()
{
    MapOperations *mapOperation = new MapOperations(m_uvMap->getMat());
    m_uvMap->setMat(mapOperation->despeckle());

    delete mapOperation;

    m_glWidget->repaint();
}

void MainWindow::filterFillHoles()
{
    MapOperations *mapOperation = new MapOperations(m_uvMap->getMat());
    m_uvMap->setMat(mapOperation->fillHoles());

    delete mapOperation;

    m_glWidget->repaint();
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
    m_glWidget->zoomInOut(true);
}

void MainWindow::zoomOut()
{
    m_glWidget->zoomInOut(false);
}

void MainWindow::zoomReset()
{
    m_glWidget->setZoom(1.0);
}

void MainWindow::zoomToFit()
{
    m_glWidget->setZoom(0.0);
}

void MainWindow::selectDisplayTexture()
{
    QAction *action = (QAction *)sender();
    if( !action )
        return;

    m_displayTexture->makeTexture((DisplayTextureManager::DISPLAY_TYPE)m_displayActionGroup->actions().indexOf(action));
    m_glWidget->setDisplayTexture(m_displayTexture->getTexture());
}

void MainWindow::toggleShowTransparency(bool value)
{
    m_glWidget->setShowTransparency(value);
}

void MainWindow::selectTransparencyGrid()
{
    QAction *action = (QAction *)sender();
    if( !action )
        return;

    m_glWidget->setTransparencyGrid((GLWidget::TRANSPARENCYGRID_TYPE)m_transparencyGridActionGroup->actions().indexOf(action));
}

void MainWindow::showAboutDialog()
{
    AboutDialog *aboutDialog = new AboutDialog(this);
    aboutDialog->setWindowFlags(aboutDialog->windowFlags() ^ Qt::WindowContextHelpButtonHint);
    aboutDialog->exec();
}
