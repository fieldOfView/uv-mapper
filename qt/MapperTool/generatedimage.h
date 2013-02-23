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
    GeneratedImage();
    GeneratedImage( QSize size, GLenum internalFormat = GL_RGBA8 );
    ~GeneratedImage();

    cv::Mat getMat();
    GLuint getTexture();

    void drawGradient( QColor topLeft, QColor topRight, QColor bottomLeft, QColor bottomRight );
    void drawGrid( QColor background, QColor lineColor, int cells, float lineWidth );

private:
    QGLFramebufferObject *m_fbo;
    QGLShaderProgram *m_gradientProgram;
    QGLShaderProgram *m_gridProgram;

    QVector<QVector3D> m_vertices;
    QVector<QVector2D> m_texCoords;

    void drawRect();
    void makeObject();
};

#endif // GENERATEDIMAGE_H
