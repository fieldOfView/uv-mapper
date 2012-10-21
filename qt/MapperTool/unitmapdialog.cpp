#include "unitmapdialog.h"
#include "ui_unitmapdialog.h"

#include <QDesktopWidget>

UnitmapDialog::UnitmapDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UnitmapDialog)
{
    ui->setupUi(this);
    ui->lineEditWidth->setValidator(new QIntValidator(1, 8192, this));
    ui->lineEditHeight->setValidator(new QIntValidator(1, 8192, this));

    selectPreset(0);
}

UnitmapDialog::~UnitmapDialog()
{;
    delete ui;
}

void UnitmapDialog::selectPreset(int presetNr)
{
  ;  switch(presetNr)
    {
    case 0:
        ui->lineEditWidth->setText(QString("%1").arg(QApplication::desktop()->screenGeometry().width()));
        ui->lineEditHeight->setText(QString("%1").arg(QApplication::desktop()->screenGeometry().height()));
        break;
    case 1:
        ui->lineEditWidth->setText("800");
        ui->lineEditHeight->setText("600");
        break;
    case 2:
        ui->lineEditWidth->setText("1024");
        ui->lineEditHeight->setText("768");
        break;
    case 3:
        ui->lineEditWidth->setText("1280");
        ui->lineEditHeight->setText("800");
        break;
    case 4:
        ui->lineEditWidth->setText("1280");
        ui->lineEditHeight->setText("720");
        break;
    case 5:
        ui->lineEditWidth->setText("1920");
        ui->lineEditHeight->setText("1080");
        break;
    case 6:
        ui->lineEditWidth->setText("2048");
        ui->lineEditHeight->setText("768");
        break;
    case 7:
        ui->lineEditWidth->setText("3072");
        ui->lineEditHeight->setText("768");
        break;
    case 8:
        ui->lineEditWidth->setText("2580");
        ui->lineEditHeight->setText("800");
        break;
    case 9:
        ui->lineEditWidth->setText("3840");
        ui->lineEditHeight->setText("800");
        break;
    }
}

QSize UnitmapDialog::getSizeSetting()
{
    return QSize( ui->lineEditWidth->text().toInt(), ui->lineEditHeight->text().toInt() );
}
