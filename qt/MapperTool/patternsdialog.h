#ifndef PATTERNSDIALOG_H
#define PATTERNSDIALOG_H

#include <QDialog>
#include <QSettings>

namespace Ui {
class PatternsDialog;
}

class PatternsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit PatternsDialog(QWidget *parent = 0);
    ~PatternsDialog();
    
private:
    Ui::PatternsDialog *ui;

    QSettings settings;
    QString dataPath;

private slots:
    void selectPatterns();
};

#endif // PATTERNSDIALOG_H
