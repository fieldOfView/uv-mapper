#include "inversedialog.h"
#include "ui_inversedialog.h"

#include <QDesktopWidget>
#include <QDebug>

#include <algorithm>

InverseDialog::InverseDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::InverseDialog)
{
    m_ui->setupUi(this);
    m_ui->lineEditWidth->setValidator(new QIntValidator(16, 8192, this));
    m_ui->lineEditHeight->setValidator(new QIntValidator(16, 8192, this));

    selectPreset(0);
}

InverseDialog::~InverseDialog()
{
    delete m_ui;
}

void InverseDialog::selectPreset(int presetNr)
{
    switch(presetNr)
    {
    case 0:
        m_ui->lineEditWidth->setText(QString("%1").arg(QApplication::desktop()->screenGeometry().width()));
        m_ui->lineEditHeight->setText(QString("%1").arg(QApplication::desktop()->screenGeometry().height()));
        break;
    case 1:
        m_ui->lineEditWidth->setText("800");
        m_ui->lineEditHeight->setText("600");
        break;
    case 2:
        m_ui->lineEditWidth->setText("1024");
        m_ui->lineEditHeight->setText("768");
        break;
    case 3:
        m_ui->lineEditWidth->setText("1280");
        m_ui->lineEditHeight->setText("800");
        break;
    case 4:
        m_ui->lineEditWidth->setText("1280");
        m_ui->lineEditHeight->setText("720");
        break;
    case 5:
        m_ui->lineEditWidth->setText("1920");
        m_ui->lineEditHeight->setText("1080");
        break;
    case 6:
        m_ui->lineEditWidth->setText("2048");
        m_ui->lineEditHeight->setText("768");
        break;
    case 7:
        m_ui->lineEditWidth->setText("3072");
        m_ui->lineEditHeight->setText("768");
        break;
    case 8:
        m_ui->lineEditWidth->setText("2580");
        m_ui->lineEditHeight->setText("800");
        break;
    case 9:
        m_ui->lineEditWidth->setText("3840");
        m_ui->lineEditHeight->setText("800");
        break;
    }
}

void InverseDialog::changeSize()
{
    int bits = 0, maxDimension = std::max(m_ui->lineEditWidth->text().toInt(), m_ui->lineEditHeight->text().toInt());
    maxDimension--;
    while (maxDimension >>= 1) ++bits;
    bits++;
    m_ui->spinBoxDepth->setValue(bits);
}

QSize InverseDialog::getSizeSetting()
{
    return QSize( m_ui->lineEditWidth->text().toInt(), m_ui->lineEditHeight->text().toInt() );
}

uint InverseDialog::getDepthSetting()
{
    return m_ui->spinBoxDepth->value();
}

bool InverseDialog::getCenteredSetting()
{
    return m_ui->checkCentered->isChecked();
}
