#include "generatedimage.h"

GeneratedImage::GeneratedImage(QSize size, GLenum internalFormat)
{
    // fbo and painter
    qDebug()<<"Test";
    fbo = new QGLFramebufferObject(size, QGLFramebufferObject::NoAttachment, GL_TEXTURE_2D, internalFormat);

    qDebug()<<"Test2";
    painter = new QPainter(fbo);

    // shaders
    QGLShader *vshader = new QGLShader(QGLShader::Vertex);
    vshader->compileSourceFile(":/glsl/passThru_vert.glsl");

    QGLShader *fshader = new QGLShader(QGLShader::Fragment);
    fshader->compileSourceFile(":/glsl/gradient_frag.glsl");

    gradientProgram = new QGLShaderProgram();
    gradientProgram->addShader(vshader);
    gradientProgram->addShader(fshader);
    gradientProgram->link();

    delete fshader;
    delete vshader;
}

GeneratedImage::~GeneratedImage()
{
    delete painter;
    delete fbo;
    delete gradientProgram;
}

GLuint GeneratedImage::getTexture()
{
    return fbo->texture();
}

cv::Mat GeneratedImage::getMat()
{
    // TODO: create cv::mat from gl::texture
    return cv::Mat();
}

void GeneratedImage::drawGradient( QColor topLeft, QColor topRight, QColor bottomLeft, QColor bottomRight ) {

    fbo->bind();
    gradientProgram->bind();

    gradientProgram->setUniformValue("colorTL",QVector3D( topLeft.redF(), topLeft.greenF(), topLeft.blueF() ));
    gradientProgram->setUniformValue("colorTR",QVector3D( topRight.redF(), topRight.greenF(), topRight.blueF() ));
    gradientProgram->setUniformValue("colorBL",QVector3D( bottomLeft.redF(), bottomLeft.greenF(), bottomLeft.blueF() ));
    gradientProgram->setUniformValue("colorBR",QVector3D( bottomRight.redF(), bottomRight.greenF(), bottomRight.blueF() ));

    painter->drawRect(QRect(QPoint(0,0),fbo->size()));

    gradientProgram->release();
    fbo->release();
}
