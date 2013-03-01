/********************************************************************************
** Form generated from reading UI file 'inversedialog.ui'
**
** Created: Fri Mar 1 14:52:28 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INVERSEDIALOG_H
#define UI_INVERSEDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_InverseDialog
{
public:
    QDialogButtonBox *buttonBox;
    QLineEdit *lineEditWidth;
    QLineEdit *lineEditHeight;
    QLabel *labelPresets;
    QLabel *labelWidth;
    QLabel *labelHeight;
    QComboBox *comboPresets;
    QCheckBox *checkCentered;
    QSpinBox *spinBoxDepth;
    QLabel *labelDepth;

    void setupUi(QDialog *InverseDialog)
    {
        if (InverseDialog->objectName().isEmpty())
            InverseDialog->setObjectName(QString::fromUtf8("InverseDialog"));
        InverseDialog->setWindowModality(Qt::ApplicationModal);
        InverseDialog->resize(275, 167);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(InverseDialog->sizePolicy().hasHeightForWidth());
        InverseDialog->setSizePolicy(sizePolicy);
        InverseDialog->setMinimumSize(QSize(275, 167));
        InverseDialog->setMaximumSize(QSize(275, 167));
        InverseDialog->setModal(true);
        buttonBox = new QDialogButtonBox(InverseDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(20, 120, 241, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        lineEditWidth = new QLineEdit(InverseDialog);
        lineEditWidth->setObjectName(QString::fromUtf8("lineEditWidth"));
        lineEditWidth->setGeometry(QRect(70, 56, 61, 20));
        lineEditWidth->setMaxLength(4);
        lineEditHeight = new QLineEdit(InverseDialog);
        lineEditHeight->setObjectName(QString::fromUtf8("lineEditHeight"));
        lineEditHeight->setGeometry(QRect(195, 56, 61, 20));
        lineEditHeight->setMaxLength(4);
        labelPresets = new QLabel(InverseDialog);
        labelPresets->setObjectName(QString::fromUtf8("labelPresets"));
        labelPresets->setGeometry(QRect(20, 20, 46, 13));
        labelWidth = new QLabel(InverseDialog);
        labelWidth->setObjectName(QString::fromUtf8("labelWidth"));
        labelWidth->setGeometry(QRect(20, 60, 46, 13));
        labelHeight = new QLabel(InverseDialog);
        labelHeight->setObjectName(QString::fromUtf8("labelHeight"));
        labelHeight->setGeometry(QRect(145, 60, 46, 13));
        comboPresets = new QComboBox(InverseDialog);
        comboPresets->setObjectName(QString::fromUtf8("comboPresets"));
        comboPresets->setGeometry(QRect(70, 16, 191, 22));
        checkCentered = new QCheckBox(InverseDialog);
        checkCentered->setObjectName(QString::fromUtf8("checkCentered"));
        checkCentered->setGeometry(QRect(145, 92, 111, 17));
        checkCentered->setChecked(true);
        spinBoxDepth = new QSpinBox(InverseDialog);
        spinBoxDepth->setObjectName(QString::fromUtf8("spinBoxDepth"));
        spinBoxDepth->setGeometry(QRect(70, 90, 61, 22));
        spinBoxDepth->setMinimum(4);
        spinBoxDepth->setMaximum(15);
        labelDepth = new QLabel(InverseDialog);
        labelDepth->setObjectName(QString::fromUtf8("labelDepth"));
        labelDepth->setGeometry(QRect(20, 95, 46, 13));

        retranslateUi(InverseDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), InverseDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), InverseDialog, SLOT(reject()));
        QObject::connect(comboPresets, SIGNAL(currentIndexChanged(int)), InverseDialog, SLOT(selectPreset(int)));
        QObject::connect(lineEditWidth, SIGNAL(textChanged(QString)), InverseDialog, SLOT(changeSize()));
        QObject::connect(lineEditHeight, SIGNAL(textChanged(QString)), InverseDialog, SLOT(changeSize()));

        QMetaObject::connectSlotsByName(InverseDialog);
    } // setupUi

    void retranslateUi(QDialog *InverseDialog)
    {
        InverseDialog->setWindowTitle(QApplication::translate("InverseDialog", "Create inverse from current map", 0, QApplication::UnicodeUTF8));
        lineEditWidth->setInputMask(QString());
        lineEditHeight->setInputMask(QString());
        labelPresets->setText(QApplication::translate("InverseDialog", "Preset:", 0, QApplication::UnicodeUTF8));
        labelWidth->setText(QApplication::translate("InverseDialog", "Width:", 0, QApplication::UnicodeUTF8));
        labelHeight->setText(QApplication::translate("InverseDialog", "Height:", 0, QApplication::UnicodeUTF8));
        comboPresets->clear();
        comboPresets->insertItems(0, QStringList()
         << QApplication::translate("InverseDialog", "Current screen resolution", "auto", QApplication::UnicodeUTF8)
         << QApplication::translate("InverseDialog", "SGA (800 x 600)", "800x600", QApplication::UnicodeUTF8)
         << QApplication::translate("InverseDialog", "XGA (1024 x 768)", "1024x768", QApplication::UnicodeUTF8)
         << QApplication::translate("InverseDialog", "WXGA (1280 x 800)", "1280x800", QApplication::UnicodeUTF8)
         << QApplication::translate("InverseDialog", "720p (1280 x 720)", "1280x720", QApplication::UnicodeUTF8)
         << QApplication::translate("InverseDialog", "1080p (1920 x 1080)", "1920x1080", QApplication::UnicodeUTF8)
         << QApplication::translate("InverseDialog", "Dual XGA (2048 x 768)", "2048x768", QApplication::UnicodeUTF8)
         << QApplication::translate("InverseDialog", "Triple XGA (3072 x 768)", "3072x768", QApplication::UnicodeUTF8)
         << QApplication::translate("InverseDialog", "Dual WXGA (2560 x 800)", "2560x800", QApplication::UnicodeUTF8)
         << QApplication::translate("InverseDialog", "Triple WXGA (3840 x 800)", "3840x800", QApplication::UnicodeUTF8)
        );
        checkCentered->setText(QApplication::translate("InverseDialog", "Centered patterns", 0, QApplication::UnicodeUTF8));
        spinBoxDepth->setSuffix(QApplication::translate("InverseDialog", " bits", 0, QApplication::UnicodeUTF8));
        labelDepth->setText(QApplication::translate("InverseDialog", "Depth:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class InverseDialog: public Ui_InverseDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INVERSEDIALOG_H
