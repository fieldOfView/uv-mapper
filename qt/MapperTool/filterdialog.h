#ifndef FILTERDIALOG_H
#define FILTERDIALOG_H

#include <QDialog>

namespace Ui {
class FilterDialog;
}

class FilterDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit FilterDialog(QWidget *parent = 0);
    ~FilterDialog();

    double getRadius();
    void setRadius(double radius);
    void setRadiusRange(double minRadius, double maxRadius, double stepRadius);
    
private:
    Ui::FilterDialog *m_ui;
};

#endif // FILTERDIALOG_H
