#ifndef GENERATEDIMAGE_H
#define GENERATEDIMAGE_H

#include <QSize>
#include <QColor>
#include <QtOpenGL>
#include <QGLFramebufferObject>

#include "opencv2/core/core.hpp"

class GeneratedImage
{
public:
    GeneratedImage( QSize size, GLenum internalFormat = GL_RGBA8 );
    ~GeneratedImage();

    cv::Mat getMat();
    GLuint getTexture();

    void drawGrid();
    void drawGradient( QColor topLeft, QColor topRight, QColor bottomLeft, QColor bottomRight );

private:
    QPainter *painter;
    QGLFramebufferObject *fbo;
    QGLShaderProgram *gradientProgram;
};

#endif // GENERATEDIMAGE_H
