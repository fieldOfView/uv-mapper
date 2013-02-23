#include "generatedimage.h"

GeneratedImage::GeneratedImage()
{
}

GeneratedImage::GeneratedImage(QSize size, GLenum internalFormat)
{
    // fbo and painter
    m_fbo = new QGLFramebufferObject(size, QGLFramebufferObject::NoAttachment, GL_TEXTURE_2D, internalFormat);

    // shaders
    QGLShader *vshader = new QGLShader(QGLShader::Vertex);
    vshader->compileSourceFile(":/glsl/passThru_vert.glsl");

    QGLShader *fshader = new QGLShader(QGLShader::Fragment);
    fshader->compileSourceFile(":/glsl/gradient_frag.glsl");

    m_gradientProgram = new QGLShaderProgram();
    m_gradientProgram->addShader(vshader);
    m_gradientProgram->addShader(fshader);
    m_gradientProgram->link();

    fshader->compileSourceFile(":/glsl/testGrid_frag.glsl");

    m_gridProgram = new QGLShaderProgram();
    m_gridProgram->addShader(vshader);
    m_gridProgram->addShader(fshader);
    m_gridProgram->link();

    delete fshader;
    delete vshader;

    makeObject();
}

GeneratedImage::~GeneratedImage()
{
    delete m_fbo;
    delete m_gradientProgram;
    delete m_gridProgram;
}

GLuint GeneratedImage::getTexture()
{
    return m_fbo->texture();
}

cv::Mat GeneratedImage::getMat()
{
    // TODO: create cv::mat from gl::texture
    return cv::Mat();
}

void GeneratedImage::drawGradient( QColor topLeft, QColor topRight, QColor bottomLeft, QColor bottomRight )
{
    m_gradientProgram->bind();

    m_gradientProgram->setUniformValue("colorTL",QVector3D( topLeft.redF(), topLeft.greenF(), topLeft.blueF() ));
    m_gradientProgram->setUniformValue("colorTR",QVector3D( topRight.redF(), topRight.greenF(), topRight.blueF() ));
    m_gradientProgram->setUniformValue("colorBL",QVector3D( bottomLeft.redF(), bottomLeft.greenF(), bottomLeft.blueF() ));
    m_gradientProgram->setUniformValue("colorBR",QVector3D( bottomRight.redF(), bottomRight.greenF(), bottomRight.blueF() ));

    drawRect();

    m_gradientProgram->release();
}

void GeneratedImage::drawGrid( QColor background, QColor lineColor, int cells, float lineWidth )
{
    m_gridProgram->bind();

    m_gridProgram->setUniformValue("colorBG",QVector3D( background.redF(), background.greenF(), background.blueF() ));
    m_gridProgram->setUniformValue("colorFG",QVector3D( lineColor.redF(), lineColor.greenF(), lineColor.blueF() ));
    m_gridProgram->setUniformValue("width", lineWidth);
    m_gridProgram->setUniformValue("cells", (float)cells);

    drawRect();

    m_gradientProgram->release();
}

void GeneratedImage::drawRect()
{
    m_fbo->bind();

    glClear(GL_COLOR_BUFFER_BIT);

    glViewport(0, 0, m_fbo->size().width(), m_fbo->size().height());

    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -10.0f);

    glVertexPointer(3, GL_FLOAT, 0, m_vertices.constData());
    glTexCoordPointer(2, GL_FLOAT, 0, m_texCoords.constData());
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);


    m_fbo->release();
}

void GeneratedImage::makeObject()
{
    m_texCoords.clear();
    m_vertices.clear();

    for (int j = 0; j < 4; ++j) {
        // Note that the texturecoordinates are upside down, because
        // frambufferobjects are somehow drawn to upside down in opengl
        m_texCoords.append
            (QVector2D(j == 0 || j == 3, j == 0 || j == 1));
        m_vertices.append
            (QVector3D(
                 ((j == 0 || j == 3)?1:-1),
                 ((j == 2 || j == 3)?1:-1),
                 0.0)
            );
    }
}
