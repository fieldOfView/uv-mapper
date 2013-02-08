#ifndef PATTERNSDIALOG_H
#define PATTERNSDIALOG_H

#include <QDialog>

#include "patternmanager.h"
#include "glwidget.h"

namespace Ui {
class PatternsDialog;
}

class PatternsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit PatternsDialog(QWidget *parent = 0, GLWidget *glWidget = 0);
    ~PatternsDialog();
    
private:
    Ui::PatternsDialog *ui;

    GLWidget *glWidget;
    PatternManager *patternManager;

    QString dataPath;

    GLuint texture;

private slots:
    void selectPatterns();
    void selectPatternFromList(QListWidgetItem *listItem);
};

#endif // PATTERNSDIALOG_H
