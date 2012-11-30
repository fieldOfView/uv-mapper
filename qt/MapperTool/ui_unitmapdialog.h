/********************************************************************************
** Form generated from reading UI file 'unitmapdialog.ui'
**
** Created: Fri Nov 30 17:27:09 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UNITMAPDIALOG_H
#define UI_UNITMAPDIALOG_H

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

QT_BEGIN_NAMESPACE

class Ui_UnitmapDialog
{
public:
    QDialogButtonBox *buttonBox;
    QLineEdit *lineEditWidth;
    QLineEdit *lineEditHeight;
    QCheckBox *checkBoxCropped;
    QLabel *labelPresets;
    QLabel *labelWidth;
    QLabel *labelHeight;
    QComboBox *comboPresets;

    void setupUi(QDialog *UnitmapDialog)
    {
        if (UnitmapDialog->objectName().isEmpty())
            UnitmapDialog->setObjectName(QString::fromUtf8("UnitmapDialog"));
        UnitmapDialog->setWindowModality(Qt::ApplicationModal);
        UnitmapDialog->resize(250, 166);
        UnitmapDialog->setModal(true);
        buttonBox = new QDialogButtonBox(UnitmapDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(20, 120, 211, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        lineEditWidth = new QLineEdit(UnitmapDialog);
        lineEditWidth->setObjectName(QString::fromUtf8("lineEditWidth"));
        lineEditWidth->setGeometry(QRect(70, 56, 51, 20));
        lineEditWidth->setMaxLength(4);
        lineEditHeight = new QLineEdit(UnitmapDialog);
        lineEditHeight->setObjectName(QString::fromUtf8("lineEditHeight"));
        lineEditHeight->setGeometry(QRect(180, 56, 51, 20));
        lineEditHeight->setMaxLength(4);
        checkBoxCropped = new QCheckBox(UnitmapDialog);
        checkBoxCropped->setObjectName(QString::fromUtf8("checkBoxCropped"));
        checkBoxCropped->setGeometry(QRect(20, 90, 211, 17));
        labelPresets = new QLabel(UnitmapDialog);
        labelPresets->setObjectName(QString::fromUtf8("labelPresets"));
        labelPresets->setGeometry(QRect(20, 20, 46, 13));
        labelWidth = new QLabel(UnitmapDialog);
        labelWidth->setObjectName(QString::fromUtf8("labelWidth"));
        labelWidth->setGeometry(QRect(20, 60, 46, 13));
        labelHeight = new QLabel(UnitmapDialog);
        labelHeight->setObjectName(QString::fromUtf8("labelHeight"));
        labelHeight->setGeometry(QRect(130, 60, 46, 13));
        comboPresets = new QComboBox(UnitmapDialog);
        comboPresets->setObjectName(QString::fromUtf8("comboPresets"));
        comboPresets->setGeometry(QRect(70, 16, 161, 22));

        retranslateUi(UnitmapDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), UnitmapDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), UnitmapDialog, SLOT(reject()));
        QObject::connect(comboPresets, SIGNAL(currentIndexChanged(int)), UnitmapDialog, SLOT(selectPreset(int)));

        QMetaObject::connectSlotsByName(UnitmapDialog);
    } // setupUi

    void retranslateUi(QDialog *UnitmapDialog)
    {
        UnitmapDialog->setWindowTitle(QApplication::translate("UnitmapDialog", "Create unit-map", 0, QApplication::UnicodeUTF8));
        lineEditWidth->setInputMask(QString());
        lineEditHeight->setInputMask(QString());
        checkBoxCropped->setText(QApplication::translate("UnitmapDialog", "Cropped Unit map", 0, QApplication::UnicodeUTF8));
        labelPresets->setText(QApplication::translate("UnitmapDialog", "Preset:", 0, QApplication::UnicodeUTF8));
        labelWidth->setText(QApplication::translate("UnitmapDialog", "Width:", 0, QApplication::UnicodeUTF8));
        labelHeight->setText(QApplication::translate("UnitmapDialog", "Height:", 0, QApplication::UnicodeUTF8));
        comboPresets->clear();
        comboPresets->insertItems(0, QStringList()
         << QApplication::translate("UnitmapDialog", "Current screen resolution", "auto", QApplication::UnicodeUTF8)
         << QApplication::translate("UnitmapDialog", "SGA (800 x 600)", "800x600", QApplication::UnicodeUTF8)
         << QApplication::translate("UnitmapDialog", "XGA (1024 x 768)", "1024x768", QApplication::UnicodeUTF8)
         << QApplication::translate("UnitmapDialog", "WXGA (1280 x 800)", "1280x800", QApplication::UnicodeUTF8)
         << QApplication::translate("UnitmapDialog", "720p (1280 x 720)", "1280x720", QApplication::UnicodeUTF8)
         << QApplication::translate("UnitmapDialog", "1080p (1920 x 1080)", "1920x1080", QApplication::UnicodeUTF8)
         << QApplication::translate("UnitmapDialog", "Dual XGA (2048 x 768)", "2048x768", QApplication::UnicodeUTF8)
         << QApplication::translate("UnitmapDialog", "Triple XGA (3072 x 768)", "3072x768", QApplication::UnicodeUTF8)
         << QApplication::translate("UnitmapDialog", "Dual WXGA (2560 x 800)", "2560x800", QApplication::UnicodeUTF8)
         << QApplication::translate("UnitmapDialog", "Triple WXGA (3840 x 800)", "3840x800", QApplication::UnicodeUTF8)
        );
    } // retranslateUi

};

namespace Ui {
    class UnitmapDialog: public Ui_UnitmapDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UNITMAPDIALOG_H
