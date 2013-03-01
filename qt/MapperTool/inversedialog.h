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
    uint getDepthSetting();
    bool getCenteredSetting();
    
private:
    Ui::InverseDialog *m_ui;

private slots:
    void selectPreset(int);
    void changeSize();
};

#endif // INVERSEDIALOG_H
