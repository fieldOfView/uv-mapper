#include <QFileDialog>
#include <QDebug>
#include "patternsdialog.h"
#include "ui_patternsdialog.h"

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "texturefrommat.h"

PatternsDialog::PatternsDialog(QWidget *parent, GLWidget *parentGlWidget) :
    QDialog(parent),
    ui(new Ui::PatternsDialog),
    glWidget(parentGlWidget)
{
    ui->setupUi(this);
    connect(ui->listWidget, SIGNAL(currentRowChanged(int)), this, SLOT(selectPatternFromList(int)));

    dataPath = "";  // TODO: get from settings

    glGenTextures(1, &texture);
    patternManager = new PatternManager();
    connect(patternManager, SIGNAL(fileLoaded(int)), this, SLOT(fileLoadProgress(int)));
    connect(patternManager, SIGNAL(patternSetSizeSet(int)), this, SLOT(setProgressDialogMax(int)));
}

PatternsDialog::~PatternsDialog()
{
    glWidget->setMode(GLWidget::MODE_UV);

    glDeleteTextures(1, &texture);
    //delete patternManager;
}

void PatternsDialog::selectPatterns()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open pattern images"), dataPath, "Images (*.jpg *.tif)");

    ui->listWidget->clear();
    ui->listWidget->addItems(fileNames);
    ui->listWidget->setEnabled(true);

    patternManager->clearOriginalPatterns();
    patternManager->loadFiles(fileNames);
    patternManager->thresholdImages();
}

void PatternsDialog::selectPatternFromList(int index)
{
    texture = makeTextureFromMat(*patternManager->getMat(index), texture);

    glWidget->setRawTexture(texture);
    glWidget->repaint();
}

void PatternsDialog::selectPatternFromList(QListWidgetItem *listItem)
{
    QString fileName = listItem->text();
    cv::Mat loadedMat = cv::imread(fileName.toStdString(),CV_LOAD_IMAGE_UNCHANGED);
    texture = makeTextureFromMat(loadedMat, texture);

    glWidget->setRawTexture(texture);
    glWidget->repaint();
}

void PatternsDialog::fileLoadProgress(int index) {
    if ( index == 1 ) {
        ui->progressBar->setEnabled(true);
    }
    ui->progressBar->setValue(index);

    if ( index >= ui->progressBar->maximum() ) {

        ui->progressBar->setEnabled(false);
    }
    selectPatternFromList(index-1);
}

void PatternsDialog::setProgressDialogMax(int max) {
    qDebug() << "Max=" << max;
    ui->progressBar->setMaximum(max);
}
