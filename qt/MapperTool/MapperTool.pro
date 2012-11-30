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
                          -llibjasper \
                          -llibjpeg \
                          -llibpng \
                          -llibtiff \
                          -lopencv_calib3d240 \
                          -lopencv_contrib240 \
                          -lopencv_core240 \
                          -lopencv_features2d240 \
                          -lopencv_flann240 \
                          -lopencv_gpu240 \
                          -lopencv_highgui240 \
                          -lopencv_imgproc240 \
                          -lopencv_legacy240 \
                          -lopencv_ml240 \
                          -lopencv_nonfree240 \
                          -lopencv_objdetect240 \
                          -lopencv_photo240 \
                          -lopencv_stitching240 \
                          -lopencv_ts240 \
                          -lopencv_video240 \
                          -lopencv_videostab240 \
                          -lzlib
                  PRE_TARGETDEPS += \
                          $${OPENCVLIBDIR}/libjasper.lib \
                          $${OPENCVLIBDIR}/libjpeg.lib \
                          $${OPENCVLIBDIR}/libpng.lib \
                          $${OPENCVLIBDIR}/libtiff.lib \
                          $${OPENCVLIBDIR}/opencv_calib3d240.lib \
                          $${OPENCVLIBDIR}/opencv_contrib240.lib \
                          $${OPENCVLIBDIR}/opencv_core240.lib \
                          $${OPENCVLIBDIR}/opencv_features2d240.lib \
                          $${OPENCVLIBDIR}/opencv_flann240.lib \
                          $${OPENCVLIBDIR}/opencv_gpu240.lib \
                          $${OPENCVLIBDIR}/opencv_highgui240.lib \
                          $${OPENCVLIBDIR}/opencv_imgproc240.lib \
                          $${OPENCVLIBDIR}/opencv_legacy240.lib \
                          $${OPENCVLIBDIR}/opencv_ml240.lib \
                          $${OPENCVLIBDIR}/opencv_nonfree240.lib \
                          $${OPENCVLIBDIR}/opencv_objdetect240.lib \
                          $${OPENCVLIBDIR}/opencv_photo240.lib \
                          $${OPENCVLIBDIR}/opencv_stitching240.lib \
                          $${OPENCVLIBDIR}/opencv_ts240.lib \
                          $${OPENCVLIBDIR}/opencv_video240.lib \
                          $${OPENCVLIBDIR}/opencv_videostab240.lib \
                          $${OPENCVLIBDIR}/zlib.lib
          } else {
                  LIBS += -L$${OPENCVLIBDIR} \
                          -llibjasperd \
                          -llibjpegd \
                          -llibpngd \
                          -llibtiffd \
                          -lopencv_calib3d240d \
                          -lopencv_contrib240d \
                          -lopencv_core240d \
                          -lopencv_features2d240d \
                          -lopencv_flann240d \
                          -lopencv_gpu240d \
                          -lopencv_highgui240d \
                          -lopencv_imgproc240d \
                          -lopencv_legacy240d \
                          -lopencv_ml240d \
                          -lopencv_nonfree240d \
                          -lopencv_objdetect240d \
                          -lopencv_photo240d \
                          -lopencv_stitching240d \
                          -lopencv_ts240d \
                          -lopencv_video240d \
                          -lopencv_videostab240d \
                          -lzlibd
                  PRE_TARGETDEPS += \
                          $${OPENCVLIBDIR}/libjasperd.lib \
                          $${OPENCVLIBDIR}/libjpegd.lib \
                          $${OPENCVLIBDIR}/libpngd.lib \
                          $${OPENCVLIBDIR}/libtiffd.lib \
                          $${OPENCVLIBDIR}/opencv_calib3d240d.lib \
                          $${OPENCVLIBDIR}/opencv_contrib240d.lib \
                          $${OPENCVLIBDIR}/opencv_core240d.lib \
                          $${OPENCVLIBDIR}/opencv_features2d240d.lib \
                          $${OPENCVLIBDIR}/opencv_flann240d.lib \
                          $${OPENCVLIBDIR}/opencv_gpu240d.lib \
                          $${OPENCVLIBDIR}/opencv_highgui240d.lib \
                          $${OPENCVLIBDIR}/opencv_imgproc240d.lib \
                          $${OPENCVLIBDIR}/opencv_legacy240d.lib \
                          $${OPENCVLIBDIR}/opencv_ml240d.lib \
                          $${OPENCVLIBDIR}/opencv_nonfree240d.lib \
                          $${OPENCVLIBDIR}/opencv_objdetect240d.lib \
                          $${OPENCVLIBDIR}/opencv_photo240d.lib \
                          $${OPENCVLIBDIR}/opencv_stitching240d.lib \
                          $${OPENCVLIBDIR}/opencv_ts240d.lib \
                          $${OPENCVLIBDIR}/opencv_video240d.lib \
                          $${OPENCVLIBDIR}/opencv_videostab240d.lib \
                          $${OPENCVLIBDIR}/zlibd.lib
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
