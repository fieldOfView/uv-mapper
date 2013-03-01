#include <QFileDialog>
#include <QDebug>
#include "patternsdialog.h"
#include "ui_patternsdialog.h"

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "texturefrommat.h"

PatternsDialog::PatternsDialog(QWidget *parent, GLWidget *parentGlWidget) :
    QDialog(parent),
    m_ui(new Ui::PatternsDialog),
    m_glWidget(parentGlWidget)
{
    m_ui->setupUi(this);
    connect(m_ui->listWidget, SIGNAL(currentRowChanged(int)), this, SLOT(selectPatternFromList(int)));

    m_dataPath = "";  // TODO: get from settings

    glGenTextures(1, &m_texture);
    m_patternManager = new PatternManager();
    connect(m_patternManager, SIGNAL(fileLoaded(int)), this, SLOT(fileLoadProgress(int)));
    connect(m_patternManager, SIGNAL(imgThresholded(int)), this, SLOT(fileLoadProgress(int)));
    connect(m_patternManager, SIGNAL(patternSetSizeSet(int)), this, SLOT(setProgressDialogMax(int)));
    connect(m_patternManager, SIGNAL(originalPatternsLoaded()), m_patternManager, SLOT(thresholdImages()));
}

PatternsDialog::~PatternsDialog()
{
    m_glWidget->setMode(GLWidget::MODE_UV);

    glDeleteTextures(1, &m_texture);
    //delete m_patternManager;
}

void PatternsDialog::selectPatterns()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open pattern images"), m_dataPath, "Images (*.jpg *.tif)");

    m_ui->listWidget->clear();
    m_ui->listWidget->addItems(fileNames);
    m_ui->listWidget->setEnabled(true);

    m_patternManager->clearOriginalPatterns();
    m_patternManager->loadFiles(fileNames);
    //m_patternManager->thresholdImages();
}

void PatternsDialog::selectPatternFromList(int index)
{
    m_texture = makeTextureFromMat(*m_patternManager->getMat(index), m_texture);

    m_glWidget->setRawTexture(m_texture);
    m_glWidget->repaint();
}

void PatternsDialog::selectPatternFromList(QListWidgetItem *listItem)
{
    QString fileName = listItem->text();
    cv::Mat loadedMat = cv::imread(fileName.toStdString(),CV_LOAD_IMAGE_UNCHANGED);
    m_texture = makeTextureFromMat(loadedMat, m_texture);

    m_glWidget->setRawTexture(m_texture);
    m_glWidget->repaint();
}

void PatternsDialog::fileLoadProgress(int index) {

    if ( index >= m_ui->progressBar->maximum() ) {

        m_ui->progressBar->setEnabled(false);
    }
    else {
        m_ui->progressBar->setEnabled(true);
        m_ui->progressBar->setValue(index);
    }
    selectPatternFromList(index-1);
}

void PatternsDialog::setProgressDialogMax(int max) {
    qDebug() << "Max=" << max;
    m_ui->progressBar->setMaximum(max);
}
