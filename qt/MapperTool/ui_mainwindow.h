/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Fri Nov 30 14:13:57 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionAbout;
    QAction *actionCreateUnit;
    QAction *actionCreateFromPatterns;
    QAction *actionCreateInverse;
    QAction *actionFileOpen;
    QAction *actionFileRevert;
    QAction *actionFileSave;
    QAction *actionFileSaveAs;
    QAction *actionExit;
    QAction *actionEditUndo;
    QAction *actionEditRedo;
    QAction *actionFullscreen;
    QAction *actionZoomIn;
    QAction *actionZoomOut;
    QAction *actionZoomActualSize;
    QAction *actionZoomFit;
    QAction *actionShowUV;
    QAction *actionShowU;
    QAction *actionShowV;
    QAction *actionShowAlpha;
    QAction *actionShowGrid;
    QAction *actionShowFile;
    QAction *actionFilterMedian;
    QAction *actionFilterGaussian;
    QAction *actionFilterDespeckle;
    QAction *actionFilterHoles;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuNew;
    QMenu *menuEdit;
    QMenu *menuFilter;
    QMenu *menuView;
    QMenu *menuZoom;
    QMenu *menuHelp;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(720, 480);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        actionCreateUnit = new QAction(MainWindow);
        actionCreateUnit->setObjectName(QString::fromUtf8("actionCreateUnit"));
        actionCreateFromPatterns = new QAction(MainWindow);
        actionCreateFromPatterns->setObjectName(QString::fromUtf8("actionCreateFromPatterns"));
        actionCreateInverse = new QAction(MainWindow);
        actionCreateInverse->setObjectName(QString::fromUtf8("actionCreateInverse"));
        actionFileOpen = new QAction(MainWindow);
        actionFileOpen->setObjectName(QString::fromUtf8("actionFileOpen"));
        actionFileRevert = new QAction(MainWindow);
        actionFileRevert->setObjectName(QString::fromUtf8("actionFileRevert"));
        actionFileSave = new QAction(MainWindow);
        actionFileSave->setObjectName(QString::fromUtf8("actionFileSave"));
        actionFileSave->setEnabled(true);
        actionFileSaveAs = new QAction(MainWindow);
        actionFileSaveAs->setObjectName(QString::fromUtf8("actionFileSaveAs"));
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionEditUndo = new QAction(MainWindow);
        actionEditUndo->setObjectName(QString::fromUtf8("actionEditUndo"));
        actionEditUndo->setEnabled(false);
        actionEditRedo = new QAction(MainWindow);
        actionEditRedo->setObjectName(QString::fromUtf8("actionEditRedo"));
        actionEditRedo->setEnabled(false);
        actionFullscreen = new QAction(MainWindow);
        actionFullscreen->setObjectName(QString::fromUtf8("actionFullscreen"));
        actionFullscreen->setCheckable(true);
        actionZoomIn = new QAction(MainWindow);
        actionZoomIn->setObjectName(QString::fromUtf8("actionZoomIn"));
        actionZoomOut = new QAction(MainWindow);
        actionZoomOut->setObjectName(QString::fromUtf8("actionZoomOut"));
        actionZoomActualSize = new QAction(MainWindow);
        actionZoomActualSize->setObjectName(QString::fromUtf8("actionZoomActualSize"));
        actionZoomFit = new QAction(MainWindow);
        actionZoomFit->setObjectName(QString::fromUtf8("actionZoomFit"));
        actionShowUV = new QAction(MainWindow);
        actionShowUV->setObjectName(QString::fromUtf8("actionShowUV"));
        actionShowU = new QAction(MainWindow);
        actionShowU->setObjectName(QString::fromUtf8("actionShowU"));
        actionShowV = new QAction(MainWindow);
        actionShowV->setObjectName(QString::fromUtf8("actionShowV"));
        actionShowAlpha = new QAction(MainWindow);
        actionShowAlpha->setObjectName(QString::fromUtf8("actionShowAlpha"));
        actionShowGrid = new QAction(MainWindow);
        actionShowGrid->setObjectName(QString::fromUtf8("actionShowGrid"));
        actionShowFile = new QAction(MainWindow);
        actionShowFile->setObjectName(QString::fromUtf8("actionShowFile"));
        actionFilterMedian = new QAction(MainWindow);
        actionFilterMedian->setObjectName(QString::fromUtf8("actionFilterMedian"));
        actionFilterGaussian = new QAction(MainWindow);
        actionFilterGaussian->setObjectName(QString::fromUtf8("actionFilterGaussian"));
        actionFilterDespeckle = new QAction(MainWindow);
        actionFilterDespeckle->setObjectName(QString::fromUtf8("actionFilterDespeckle"));
        actionFilterHoles = new QAction(MainWindow);
        actionFilterHoles->setObjectName(QString::fromUtf8("actionFilterHoles"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy);
        centralWidget->setCursor(QCursor(Qt::ArrowCursor));
        centralWidget->setAutoFillBackground(true);
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 720, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuNew = new QMenu(menuFile);
        menuNew->setObjectName(QString::fromUtf8("menuNew"));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QString::fromUtf8("menuEdit"));
        menuFilter = new QMenu(menuBar);
        menuFilter->setObjectName(QString::fromUtf8("menuFilter"));
        menuView = new QMenu(menuBar);
        menuView->setObjectName(QString::fromUtf8("menuView"));
        menuZoom = new QMenu(menuView);
        menuZoom->setObjectName(QString::fromUtf8("menuZoom"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuBar->addAction(menuFilter->menuAction());
        menuBar->addAction(menuView->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(menuNew->menuAction());
        menuFile->addSeparator();
        menuFile->addAction(actionFileOpen);
        menuFile->addAction(actionFileRevert);
        menuFile->addSeparator();
        menuFile->addAction(actionFileSave);
        menuFile->addAction(actionFileSaveAs);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuNew->addAction(actionCreateUnit);
        menuNew->addAction(actionCreateFromPatterns);
        menuNew->addAction(actionCreateInverse);
        menuEdit->addAction(actionEditUndo);
        menuEdit->addAction(actionEditRedo);
        menuFilter->addAction(actionFilterMedian);
        menuFilter->addAction(actionFilterGaussian);
        menuFilter->addAction(actionFilterDespeckle);
        menuFilter->addSeparator();
        menuFilter->addAction(actionFilterHoles);
        menuView->addAction(actionFullscreen);
        menuView->addAction(menuZoom->menuAction());
        menuView->addSeparator();
        menuView->addAction(actionShowUV);
        menuView->addAction(actionShowU);
        menuView->addAction(actionShowV);
        menuView->addAction(actionShowAlpha);
        menuView->addSeparator();
        menuView->addAction(actionShowGrid);
        menuView->addAction(actionShowFile);
        menuZoom->addAction(actionZoomIn);
        menuZoom->addAction(actionZoomOut);
        menuZoom->addSeparator();
        menuZoom->addAction(actionZoomActualSize);
        menuZoom->addAction(actionZoomFit);
        menuHelp->addAction(actionAbout);

        retranslateUi(MainWindow);
        QObject::connect(actionExit, SIGNAL(triggered()), MainWindow, SLOT(close()));
        QObject::connect(actionFullscreen, SIGNAL(triggered()), MainWindow, SLOT(toggleFullscreen()));
        QObject::connect(actionAbout, SIGNAL(triggered()), MainWindow, SLOT(showAboutDialog()));
        QObject::connect(actionCreateUnit, SIGNAL(triggered()), MainWindow, SLOT(showUnitmapDialog()));
        QObject::connect(actionFileOpen, SIGNAL(triggered()), MainWindow, SLOT(fileOpen()));
        QObject::connect(actionFileRevert, SIGNAL(triggered()), MainWindow, SLOT(fileRevert()));
        QObject::connect(actionFileSave, SIGNAL(triggered()), MainWindow, SLOT(fileSave()));
        QObject::connect(actionFileSaveAs, SIGNAL(triggered()), MainWindow, SLOT(fileSaveAs()));
        QObject::connect(actionShowUV, SIGNAL(triggered()), MainWindow, SLOT(selectDisplayTexture()));
        QObject::connect(actionShowU, SIGNAL(triggered()), MainWindow, SLOT(selectDisplayTexture()));
        QObject::connect(actionShowV, SIGNAL(triggered()), MainWindow, SLOT(selectDisplayTexture()));
        QObject::connect(actionShowAlpha, SIGNAL(triggered()), MainWindow, SLOT(selectDisplayTexture()));
        QObject::connect(actionShowGrid, SIGNAL(triggered()), MainWindow, SLOT(selectDisplayTexture()));
        QObject::connect(actionShowFile, SIGNAL(triggered()), MainWindow, SLOT(selectDisplayTexture()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Mapper Tool", 0, QApplication::UnicodeUTF8));
        actionAbout->setText(QApplication::translate("MainWindow", "About...", 0, QApplication::UnicodeUTF8));
        actionCreateUnit->setText(QApplication::translate("MainWindow", "Unit map...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionCreateUnit->setToolTip(QApplication::translate("MainWindow", "Create a new \"Unit Map\"", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionCreateUnit->setShortcut(QApplication::translate("MainWindow", "Ctrl+N", 0, QApplication::UnicodeUTF8));
        actionCreateFromPatterns->setText(QApplication::translate("MainWindow", "From patterns...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionCreateFromPatterns->setToolTip(QApplication::translate("MainWindow", "Create a map from a series of captured patters", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionCreateFromPatterns->setShortcut(QApplication::translate("MainWindow", "Ctrl+Shift+O", 0, QApplication::UnicodeUTF8));
        actionCreateInverse->setText(QApplication::translate("MainWindow", "Inverse from current", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionCreateInverse->setToolTip(QApplication::translate("MainWindow", "Create a inverse map from the currently open map", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionCreateInverse->setShortcut(QApplication::translate("MainWindow", "Ctrl+I", 0, QApplication::UnicodeUTF8));
        actionFileOpen->setText(QApplication::translate("MainWindow", "Open...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionFileOpen->setToolTip(QApplication::translate("MainWindow", "Load a map from disk", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionFileOpen->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", 0, QApplication::UnicodeUTF8));
        actionFileRevert->setText(QApplication::translate("MainWindow", "Revert to saved", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionFileRevert->setToolTip(QApplication::translate("MainWindow", "Revert to the version saved to disk", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionFileSave->setText(QApplication::translate("MainWindow", "Save", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionFileSave->setToolTip(QApplication::translate("MainWindow", "Save current map to disk", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionFileSave->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", 0, QApplication::UnicodeUTF8));
        actionFileSaveAs->setText(QApplication::translate("MainWindow", "Save as...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionFileSaveAs->setToolTip(QApplication::translate("MainWindow", "Save current map under a different filename", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionFileSaveAs->setShortcut(QApplication::translate("MainWindow", "Ctrl+Shift+S", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("MainWindow", "Exit", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionExit->setToolTip(QApplication::translate("MainWindow", "Quit the application", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionEditUndo->setText(QApplication::translate("MainWindow", "Undo", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionEditUndo->setToolTip(QApplication::translate("MainWindow", "Undo last action", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionEditUndo->setShortcut(QApplication::translate("MainWindow", "Ctrl+Z", 0, QApplication::UnicodeUTF8));
        actionEditRedo->setText(QApplication::translate("MainWindow", "Redo", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionEditRedo->setToolTip(QApplication::translate("MainWindow", "Redo the last action", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionEditRedo->setShortcut(QApplication::translate("MainWindow", "Ctrl+Shift+Z", 0, QApplication::UnicodeUTF8));
        actionFullscreen->setText(QApplication::translate("MainWindow", "Full Screen", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionFullscreen->setToolTip(QApplication::translate("MainWindow", "Toggle Full Screen mode", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionFullscreen->setShortcut(QApplication::translate("MainWindow", "Ctrl+F", 0, QApplication::UnicodeUTF8));
        actionZoomIn->setText(QApplication::translate("MainWindow", "Zoom In", 0, QApplication::UnicodeUTF8));
        actionZoomIn->setShortcut(QApplication::translate("MainWindow", "Ctrl+=", 0, QApplication::UnicodeUTF8));
        actionZoomOut->setText(QApplication::translate("MainWindow", "Zoom Out", 0, QApplication::UnicodeUTF8));
        actionZoomOut->setShortcut(QApplication::translate("MainWindow", "Ctrl+-", 0, QApplication::UnicodeUTF8));
        actionZoomActualSize->setText(QApplication::translate("MainWindow", "Actual Size", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionZoomActualSize->setToolTip(QApplication::translate("MainWindow", "Show the current map at 100%", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionZoomActualSize->setShortcut(QApplication::translate("MainWindow", "Ctrl+1", 0, QApplication::UnicodeUTF8));
        actionZoomFit->setText(QApplication::translate("MainWindow", "Fit to window", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionZoomFit->setToolTip(QApplication::translate("MainWindow", "Fit the current map to the window size", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionZoomFit->setShortcut(QApplication::translate("MainWindow", "Ctrl+0", 0, QApplication::UnicodeUTF8));
        actionShowUV->setText(QApplication::translate("MainWindow", "UV gradient", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionShowUV->setToolTip(QApplication::translate("MainWindow", "Show the native UV gradient of the current map", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionShowU->setText(QApplication::translate("MainWindow", "U gradient", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionShowU->setToolTip(QApplication::translate("MainWindow", "Show only the U gradient of the current map", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionShowV->setText(QApplication::translate("MainWindow", "V gradient", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionShowV->setToolTip(QApplication::translate("MainWindow", "Show only the V gradient of the current map", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionShowAlpha->setText(QApplication::translate("MainWindow", "Alpha channel", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionShowAlpha->setToolTip(QApplication::translate("MainWindow", "Show the alpha channel of the current map", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionShowGrid->setText(QApplication::translate("MainWindow", "Grid", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionShowGrid->setToolTip(QApplication::translate("MainWindow", "Transform a grid image with the current map", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionShowFile->setText(QApplication::translate("MainWindow", "File...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionShowFile->setToolTip(QApplication::translate("MainWindow", "Open an image or movie and transform it with the current map", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionFilterMedian->setText(QApplication::translate("MainWindow", "Median...", 0, QApplication::UnicodeUTF8));
        actionFilterGaussian->setText(QApplication::translate("MainWindow", "Gaussian...", 0, QApplication::UnicodeUTF8));
        actionFilterDespeckle->setText(QApplication::translate("MainWindow", "Despeckle...", 0, QApplication::UnicodeUTF8));
        actionFilterHoles->setText(QApplication::translate("MainWindow", "Fill holes...", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0, QApplication::UnicodeUTF8));
        menuNew->setTitle(QApplication::translate("MainWindow", "New", 0, QApplication::UnicodeUTF8));
        menuEdit->setTitle(QApplication::translate("MainWindow", "Edit", 0, QApplication::UnicodeUTF8));
        menuFilter->setTitle(QApplication::translate("MainWindow", "Filter", 0, QApplication::UnicodeUTF8));
        menuView->setTitle(QApplication::translate("MainWindow", "View", 0, QApplication::UnicodeUTF8));
        menuZoom->setTitle(QApplication::translate("MainWindow", "Zoom", 0, QApplication::UnicodeUTF8));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
