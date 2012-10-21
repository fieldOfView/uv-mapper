#ifndef UNITMAPDIALOG_H
#define UNITMAPDIALOG_H

#include <QDialog>

namespace Ui {
    class UnitmapDialog;
}

class UnitmapDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit UnitmapDialog(QWidget *parent = 0);
    ~UnitmapDialog();

    QSize getSizeSetting();
    
private:
    Ui::UnitmapDialog *ui;

private slots:
    void selectPreset(int);
};

#endif // UNITMAPDIALOG_H
