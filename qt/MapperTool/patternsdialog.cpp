#include "patternsdialog.h"
#include "ui_patternsdialog.h"

PatternsDialog::PatternsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PatternsDialog)
{
    ui->setupUi(this);
}

PatternsDialog::~PatternsDialog()
{
    delete ui;
}
