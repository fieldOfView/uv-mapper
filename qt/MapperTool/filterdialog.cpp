#include "filterdialog.h"
#include "ui_filterdialog.h"

FilterDialog::FilterDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::FilterDialog)
{
    m_ui->setupUi(this);
}

FilterDialog::~FilterDialog()
{
    delete m_ui;
}

double FilterDialog::getRadius()
{
    return (double)( m_ui->spinBox->value());
}

void FilterDialog::setRadius(double radius)
{
    m_ui->spinBox->setValue((int)radius);
}

void FilterDialog::setRadiusRange(double minRadius, double maxRadius, double stepRadius)
{
    m_ui->spinBox->setRange((int)minRadius, (int)maxRadius);
    m_ui->spinBox->setSingleStep((int)stepRadius);
    m_ui->horizontalSlider->setRange((int)minRadius, (int)maxRadius);
    m_ui->horizontalSlider->setSingleStep((int)stepRadius);
}
