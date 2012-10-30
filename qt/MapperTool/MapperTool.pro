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
    unitmapdialog.cpp

HEADERS  += mainwindow.h \
    aboutdialog.h \
    glwidget.h \
    unitmapdialog.h

FORMS    += mainwindow.ui \
    aboutdialog.ui \
    unitmapdialog.ui


win32 {
  OPENCVDIR = "../../../opencv/build"
  OPENCVLIBDIR = $${OPENCVDIR}/x86/vc10/staticlib
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
                          -lopencv_calib3d243 \
                          -lopencv_contrib243 \
                          -lopencv_core243 \
                          -lopencv_features2d243 \
                          -lopencv_flann243 \
                          -lopencv_gpu243 \
                          -lopencv_highgui243 \
                          -lopencv_imgproc243 \
                          -lopencv_legacy243 \
                          -lopencv_ml243 \
                          -lopencv_nonfree243 \
                          -lopencv_objdetect243 \
                          -lopencv_photo243 \
                          -lopencv_stitching243 \
                          -lopencv_ts243 \
                          -lopencv_video243 \
                          -lopencv_videostab243 \
                          -lzlib
                  PRE_TARGETDEPS += \
                          $${OPENCVLIBDIR}/libjasper.lib \
                          $${OPENCVLIBDIR}/libjpeg.lib \
                          $${OPENCVLIBDIR}/libpng.lib \
                          $${OPENCVLIBDIR}/libtiff.lib \
                          $${OPENCVLIBDIR}/opencv_calib3d243.lib \
                          $${OPENCVLIBDIR}/opencv_contrib243.lib \
                          $${OPENCVLIBDIR}/opencv_core243.lib \
                          $${OPENCVLIBDIR}/opencv_features2d243.lib \
                          $${OPENCVLIBDIR}/opencv_flann243.lib \
                          $${OPENCVLIBDIR}/opencv_gpu243.lib \
                          $${OPENCVLIBDIR}/opencv_highgui243.lib \
                          $${OPENCVLIBDIR}/opencv_imgproc243.lib \
                          $${OPENCVLIBDIR}/opencv_legacy243.lib \
                          $${OPENCVLIBDIR}/opencv_ml243.lib \
                          $${OPENCVLIBDIR}/opencv_nonfree243.lib \
                          $${OPENCVLIBDIR}/opencv_objdetect243.lib \
                          $${OPENCVLIBDIR}/opencv_photo243.lib \
                          $${OPENCVLIBDIR}/opencv_stitching243.lib \
                          $${OPENCVLIBDIR}/opencv_ts243.lib \
                          $${OPENCVLIBDIR}/opencv_video243.lib \
                          $${OPENCVLIBDIR}/opencv_videostab243.lib \
                          $${OPENCVLIBDIR}/zlib.lib
          } else {
                  LIBS += -L$${OPENCVLIBDIR} \
                          -llibjasperd \
                          -llibjpegd \
                          -llibpngd \
                          -llibtiffd \
                          -lopencv_calib3d243d \
                          -lopencv_contrib243d \
                          -lopencv_core243d \
                          -lopencv_features2d243d \
                          -lopencv_flann243d \
                          -lopencv_gpu243d \
                          -lopencv_highgui243d \
                          -lopencv_imgproc243d \
                          -lopencv_legacy243d \
                          -lopencv_ml243d \
                          -lopencv_nonfree243d \
                          -lopencv_objdetect243d \
                          -lopencv_photo243d \
                          -lopencv_stitching243d \
                          -lopencv_ts243d \
                          -lopencv_video243d \
                          -lopencv_videostab243d \
                          -lzlibd
                  PRE_TARGETDEPS += \
                          $${OPENCVLIBDIR}/libjasperd.lib \
                          $${OPENCVLIBDIR}/libjpegd.lib \
                          $${OPENCVLIBDIR}/libpngd.lib \
                          $${OPENCVLIBDIR}/libtiffd.lib \
                          $${OPENCVLIBDIR}/opencv_calib3d243d.lib \
                          $${OPENCVLIBDIR}/opencv_contrib243d.lib \
                          $${OPENCVLIBDIR}/opencv_core243d.lib \
                          $${OPENCVLIBDIR}/opencv_features2d243d.lib \
                          $${OPENCVLIBDIR}/opencv_flann243d.lib \
                          $${OPENCVLIBDIR}/opencv_gpu243d.lib \
                          $${OPENCVLIBDIR}/opencv_highgui243d.lib \
                          $${OPENCVLIBDIR}/opencv_imgproc243d.lib \
                          $${OPENCVLIBDIR}/opencv_legacy243d.lib \
                          $${OPENCVLIBDIR}/opencv_ml243d.lib \
                          $${OPENCVLIBDIR}/opencv_nonfree243d.lib \
                          $${OPENCVLIBDIR}/opencv_objdetect243d.lib \
                          $${OPENCVLIBDIR}/opencv_photo243d.lib \
                          $${OPENCVLIBDIR}/opencv_stitching243d.lib \
                          $${OPENCVLIBDIR}/opencv_ts243d.lib \
                          $${OPENCVLIBDIR}/opencv_video243d.lib \
                          $${OPENCVLIBDIR}/opencv_videostab243d.lib \
                          $${OPENCVLIBDIR}/zlibd.lib
          }
          message("OpenCV libraries found in $${OPENCVDIR}")
  } else {
          message("OpenCV libraries not found.")
  }
}
