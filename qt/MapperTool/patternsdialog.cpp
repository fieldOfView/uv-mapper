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

    dataPath = "";  // TODO: get from settings

    glGenTextures(1, &texture);
    //patternManager = new PatternManager();
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
}

void PatternsDialog::selectPatternFromList(QListWidgetItem *listItem)
{
    QString fileName = listItem->text();
    cv::Mat loadedMat = cv::imread(fileName.toStdString(),CV_LOAD_IMAGE_UNCHANGED);
    texture = makeTextureFromMat(loadedMat, texture);
    loadedMat = 0;

    glWidget->setRawTexture(texture);
    glWidget->repaint();
}


