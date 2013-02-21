#ifndef TEXTUREFROMMAT_H
#define TEXTUREFROMMAT_H

#include <QtOpenGL>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

GLuint makeTextureFromMat(cv::Mat& mat, GLuint texture);

#endif // TEXTUREFROMMAT_H
