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

signals:

    
private:
    Ui::PatternsDialog *m_ui;

    GLWidget *m_glWidget;
    PatternManager *m_patternManager;

    QString m_dataPath;

    GLuint m_texture;

private slots:
    void selectPatterns();
    void selectPatternFromList(QListWidgetItem *listItem);
    void selectPatternFromList(int index);
    void fileLoadProgress(int index);
    void setProgressDialogMax(int max);
};

#endif // PATTERNSDIALOG_H
