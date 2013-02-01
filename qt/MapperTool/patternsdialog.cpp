#include <QFileDialog>
#include <QDebug>
#include "patternsdialog.h"
#include "ui_patternsdialog.h"

PatternsDialog::PatternsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PatternsDialog)
{
    dataPath = settings.value("DataPath").toString();

    ui->setupUi(this);
}

PatternsDialog::~PatternsDialog()
{
    delete ui;
}

void PatternsDialog::selectPatterns()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open pattern images"), dataPath, "Images (*.jpg *.tif)");
    qDebug()<<fileNames;
}
