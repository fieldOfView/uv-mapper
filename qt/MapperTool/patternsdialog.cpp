#include <QFileDialog>
#include <QDebug>
#include "patternsdialog.h"
#include "ui_patternsdialog.h"
#include "mt_graydecoder.h"

PatternsDialog::PatternsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PatternsDialog)
{
    ui->setupUi(this);

    dataPath = "";  // TODO: get from settings
    QStringList m_fileNames;
    m_fileNames.push_back("/home/people/arnaud/Downloads/1 - raw photos/_DSC3899.tif");
    m_fileNames.push_back("/home/people/arnaud/Downloads/1 - raw photos/_DSC3890.tif");
    m_fileNames.push_back("/home/people/arnaud/Downloads/1 - raw photos/_DSC3888.tif");
    m_fileNames.push_back("/home/people/arnaud/Downloads/1 - raw photos/_DSC3904.tif");
    m_fileNames.push_back("/home/people/arnaud/Downloads/1 - raw photos/_DSC3901.tif");
    m_fileNames.push_back("/home/people/arnaud/Downloads/1 - raw photos/_DSC3884.tif");
    m_fileNames.push_back("/home/people/arnaud/Downloads/1 - raw photos/_DSC3885.tif");
    m_fileNames.push_back("/home/people/arnaud/Downloads/1 - raw photos/_DSC3896.tif");
    m_fileNames.push_back("/home/people/arnaud/Downloads/1 - raw photos/_DSC3882.tif");
    m_fileNames.push_back("/home/people/arnaud/Downloads/1 - raw photos/_DSC3903.tif");
    m_fileNames.push_back("/home/people/arnaud/Downloads/1 - raw photos/_DSC3887.tif");
    m_fileNames.push_back("/home/people/arnaud/Downloads/1 - raw photos/_DSC3898.tif");
    m_fileNames.push_back("/home/people/arnaud/Downloads/1 - raw photos/_DSC3894.tif");
    m_fileNames.push_back("/home/people/arnaud/Downloads/1 - raw photos/_DSC3891.tif");
    m_fileNames.push_back("/home/people/arnaud/Downloads/1 - raw photos/_DSC3886.tif");
    m_fileNames.push_back("/home/people/arnaud/Downloads/1 - raw photos/_DSC3893.tif");
    m_fileNames.push_back("/home/people/arnaud/Downloads/1 - raw photos/_DSC3905.tif");
    m_fileNames.push_back("/home/people/arnaud/Downloads/1 - raw photos/_DSC3897.tif");
    m_fileNames.push_back("/home/people/arnaud/Downloads/1 - raw photos/_DSC3895.tif");
    m_fileNames.push_back("/home/people/arnaud/Downloads/1 - raw photos/_DSC3892.tif");
    m_fileNames.push_back("/home/people/arnaud/Downloads/1 - raw photos/_DSC3902.tif");
    m_fileNames.push_back("/home/people/arnaud/Downloads/1 - raw photos/_DSC3889.tif");
    m_fileNames.push_back("/home/people/arnaud/Downloads/1 - raw photos/_DSC3900.tif");
    m_fileNames.push_back("/home/people/arnaud/Downloads/1 - raw photos/_DSC3883.tif");
    patternManager = new PatternManager();
    patternManager->loadFiles(m_fileNames);
    patternManager->thresholdImages();
}

PatternsDialog::~PatternsDialog()
{
    delete ui;
    delete patternManager;
}

void PatternsDialog::selectPatterns()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open pattern images"), dataPath, "Images (*.jpg *.tif)");

    ui->listWidget->clear();
    ui->listWidget->addItems(fileNames);
    ui->listWidget->setEnabled(true);
}
