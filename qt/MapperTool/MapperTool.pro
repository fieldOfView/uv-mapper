#-------------------------------------------------
#
# Project created by QtCreator 2012-10-19T12:51:14
#
#-------------------------------------------------

QT += \
    core gui \
    opengl

TARGET = MapperTool
TEMPLATE = app

SOURCES += \
    main.cpp\
    mainwindow.cpp \
    aboutdialog.cpp \
    glwidget.cpp \
    unitmapdialog.cpp \
    mapmanager.cpp \
    displaytexturemanager.cpp \
    generatedimage.cpp \
    patternmanager.cpp \
    mt_graydecoder.cpp \
    patternsdialog.cpp \
    texturefrommat.cpp

HEADERS += \
    mainwindow.h \
    aboutdialog.h \
    glwidget.h \
    unitmapdialog.h \
    mapmanager.h \
    displaytexturemanager.h \
    generatedimage.h \
    threadSafeQueue.h \
    patternmanager.h \
    mt_graydecoder.h \
    patternsdialog.h \
    texturefrommat.h

FORMS += \
    mainwindow.ui \
    aboutdialog.ui \
    unitmapdialog.ui \
    patternsdialog.ui

RESOURCES += \
    resources.qrc

OTHER_FILES += \
    resources/uvMap_frag.glsl \
    resources/passThru_vert.glsl \
    resources/transparencyGrid_frag.glsl \
    resources/gradient_frag.glsl \
    resources/testGrid_frag.glsl

unix {
        LIBS += -I/usr/include/opencv -lopencv_core -lopencv_highgui
}

macx {
    # Include and link opencv dependency

    OPENCVDIR = "../../../opencv/install"
    OPENCVINCLUDEDIR = $${OPENCVDIR}/include
    OPENCVLIBDIR = $${OPENCVDIR}/lib
    exists($$OPENCVDIR) {
          DEFINES += USEOPENCV
          INCLUDEPATH += $${OPENCVINCLUDEDIR}
          CONFIG(release, debug|release) {
                  LIBS += -L$${OPENCVLIBDIR} \
                          -lopencv_core240 \
                          -lopencv_highgui240
          } else {
                  LIBS += -L$${OPENCVLIBDIR} \
                          -lopencv_core240d \
                          -lopencv_highgui240d
          }
    } else {
          message("OpenCV libraries not found.")
    }
}

win32 {
    # Copy Qt runtime

    CONFIG(debug, debug|release) {
        QtCored4.commands = copy /Y %QTDIR%\\bin\\QtCored4.dll debug
        QtCored4.target = debug/QtCored4.dll
        QtGuid4.commands = copy /Y %QTDIR%\\bin\\QtGuid4.dll debug
        QtGuid4.target = debug/QtGuid4.dll
        QtOpenGLd4.commands = copy /Y %QTDIR%\\bin\\QtOpenGLd4.dll debug
        QtOpenGLd4.target = debug/QtOpenGLd4.dll

        QMAKE_EXTRA_TARGETS += QtCored4 QtGuid4 QtOpenGLd4
        PRE_TARGETDEPS += debug/QtCored4.dll debug/QtGuid4.dll debug/QtOpenGLd4.dll
    } else {
        QtCore4.commands = copy /Y %QTDIR%\\bin\\QtCore4.dll release
        QtCore4.target = release/QtCore4.dll
        QtGui4.commands = copy /Y %QTDIR%\\bin\\QtGui4.dll release
        QtGui4.target = release/QtGui4.dll
        QtOpenGL4.commands = copy /Y %QTDIR%\\bin\\QtOpenGL4.dll release
        QtOpenGL4.target = release/QtOpenGL4.dll

        QMAKE_EXTRA_TARGETS += QtCore4 QtGui4 QtOpenGL4
        PRE_TARGETDEPS += release/QtCore4.dll release/QtGui4.dll release/QtOpenGL4.dll
    }

    # Include, link and copy opencv dependency

    OPENCVDIR = "..\\..\\..\\opencv\\install"
    OPENCVINCLUDEDIR = $${OPENCVDIR}\\include
    OPENCVLIBDIR = $${OPENCVDIR}\\lib
    OPENCVBINDIR = $${OPENCVDIR}\\bin
    exists($$OPENCVDIR) {
          DEFINES += USEOPENCV
          INCLUDEPATH += $${OPENCVINCLUDEDIR}
          DEPENDPATH += $${OPENCVINCLUDEDIR}
          CONFIG(release, debug|release) {
                  LIBS += -L$${OPENCVLIBDIR} \
                          -lopencv_core240 \
                          -lopencv_highgui240
                  PRE_TARGETDEPS += \
                          $${OPENCVLIBDIR}/opencv_core240.lib \
                          $${OPENCVLIBDIR}/opencv_highgui240.lib

                  opencv_core240.commands = copy /Y $${OPENCVBINDIR}\\opencv_core240.dll release
                  opencv_core240.target = release/opencv_core240.dll
                  opencv_highgui240.commands = copy /Y $${OPENCVBINDIR}\\opencv_highgui240.dll release
                  opencv_highgui240.target = release/opencv_highgui240.dll

                  QMAKE_EXTRA_TARGETS += opencv_core240 opencv_highgui240
                  PRE_TARGETDEPS += release/opencv_core240.dll release/opencv_highgui240.dll

          } else {
                  LIBS += -L$${OPENCVLIBDIR} \
                          -lopencv_core240d \
                          -lopencv_highgui240d
                  PRE_TARGETDEPS += \
                          $${OPENCVLIBDIR}/opencv_core240d.lib \
                          $${OPENCVLIBDIR}/opencv_highgui240d.lib

                  opencv_core240d.commands = copy /Y $${OPENCVBINDIR}\\opencv_core240d.dll debug
                  opencv_core240d.target = debug/opencv_core240d.dll
                  opencv_highgui240d.commands = copy /Y $${OPENCVBINDIR}\\opencv_highgui240d.dll debug
                  opencv_highgui240d.target = debug/opencv_highgui240d.dll

                  QMAKE_EXTRA_TARGETS += opencv_core240d opencv_highgui240d
                  PRE_TARGETDEPS += debug/opencv_core240d.dll debug/opencv_highgui240d.dll

          }
    } else {
          message("OpenCV libraries not found.")
    }
}

