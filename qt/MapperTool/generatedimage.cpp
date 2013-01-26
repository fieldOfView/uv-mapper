#include "generatedimage.h"

GeneratedImage::GeneratedImage(QSize size, GLenum internalFormat)
{
    // fbo and painter
    fbo = new QGLFramebufferObject(size, QGLFramebufferObject::NoAttachment, GL_TEXTURE_2D, internalFormat);

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

    makeObject();
}

GeneratedImage::~GeneratedImage()
{
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

    glClear(GL_COLOR_BUFFER_BIT);

    glViewport(0, 0, fbo->size().width(), fbo->size().height());

    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -10.0f);

    glVertexPointer(3, GL_FLOAT, 0, vertices.constData());
    glTexCoordPointer(2, GL_FLOAT, 0, texCoords.constData());
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    gradientProgram->bind();

    gradientProgram->setUniformValue("colorTL",QVector3D( topLeft.redF(), topLeft.greenF(), topLeft.blueF() ));
    gradientProgram->setUniformValue("colorTR",QVector3D( topRight.redF(), topRight.greenF(), topRight.blueF() ));
    gradientProgram->setUniformValue("colorBL",QVector3D( bottomLeft.redF(), bottomLeft.greenF(), bottomLeft.blueF() ));
    gradientProgram->setUniformValue("colorBR",QVector3D( bottomRight.redF(), bottomRight.greenF(), bottomRight.blueF() ));

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    gradientProgram->release();

    fbo->release();
}

void GeneratedImage::makeObject()
{
    texCoords.clear();
    vertices.clear();

    for (int j = 0; j < 4; ++j) {
        // Note that the texturecoordinates are upside down, because
        // frambufferobjects are somehow drawn to upside down in opengl
        texCoords.append
            (QVector2D(j == 0 || j == 3, j == 0 || j == 1));
        vertices.append
            (QVector3D(
                 ((j == 0 || j == 3)?1:-1),
                 ((j == 2 || j == 3)?1:-1),
                 0.0)
            );
    }
}
