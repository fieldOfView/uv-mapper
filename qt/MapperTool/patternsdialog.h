#ifndef PATTERNSDIALOG_H
#define PATTERNSDIALOG_H

#include <QDialog>

#include "patternmanager.h"

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

    PatternManager *patternManager;

    QString dataPath;

private slots:
    void selectPatterns();
};

#endif // PATTERNSDIALOG_H
