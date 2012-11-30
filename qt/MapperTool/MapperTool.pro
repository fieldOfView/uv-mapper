#-------------------------------------------------
#
# Project created by QtCreator 2012-10-19T12:51:14
#
#-------------------------------------------------

QT       += core gui \
    opengl

TARGET = MapperTool
TEMPLATE = app

SOURCES += main.cpp\
    mainwindow.cpp \
    aboutdialog.cpp \
    glwidget.cpp \
    unitmapdialog.cpp \
    mapmanager.cpp \
    displaytexturemanager.cpp

HEADERS  += mainwindow.h \
    aboutdialog.h \
    glwidget.h \
    unitmapdialog.h \
    mapmanager.h \
    displaytexturemanager.h

FORMS    += mainwindow.ui \
    aboutdialog.ui \
    unitmapdialog.ui


win32 {
  OPENCVDIR = "../../../opencv/install"
  OPENCVLIBDIR = $${OPENCVDIR}/lib
  exists($$OPENCVDIR) {
          DEFINES += USEOPENCV
          INCLUDEPATH += \
                  $${OPENCVDIR}/include
          DEPENDPATH += \
                  $${OPENCVDIR}/include
          CONFIG(release, debug|release) {
                  LIBS += -L$${OPENCVLIBDIR} \
                          -lopencv_core240 \
                          -lopencv_highgui240
                  PRE_TARGETDEPS += \
                          $${OPENCVLIBDIR}/opencv_core240.lib \
                          $${OPENCVLIBDIR}/opencv_highgui240.lib
          } else {
                  LIBS += -L$${OPENCVLIBDIR} \
                          -lopencv_core240d \
                          -lopencv_highgui240d
                  PRE_TARGETDEPS += \
                          $${OPENCVLIBDIR}/opencv_core240d.lib \
                          $${OPENCVLIBDIR}/opencv_highgui240d.lib
          }
          message("OpenCV libraries found in $${OPENCVDIR}")
  } else {
          message("OpenCV libraries not found.")
  }
}

RESOURCES += \
    resources.qrc

OTHER_FILES += \
    resources/uvMap_frag.glsl \
    resources/passThru_vert.glsl
