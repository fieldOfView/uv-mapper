#ifndef INVERSEDIALOG_H
#define INVERSEDIALOG_H

#include <QDialog>

namespace Ui {
class InverseDialog;
}

class InverseDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit InverseDialog(QWidget *parent = 0);
    ~InverseDialog();

    QSize getSizeSetting();
    
private:
    Ui::InverseDialog *m_ui;

private slots:
    void selectPreset(int);
};

#endif // INVERSEDIALOG_H
