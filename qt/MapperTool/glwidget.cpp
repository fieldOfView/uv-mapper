/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
** $QT_END_LICENSE$
**
****************************************************************************/

#include "glwidget.h"
#include <QtOpenGL>

GLWidget::GLWidget(QWidget *parent, QGLWidget *shareWidget)
    : QGLWidget(parent, shareWidget),
      m_mapSize(1,1),m_rawSize(1,1),
      m_mapTexture(0),m_rawTexture(0),
      m_zoomFactor(1.0),
      m_showTransparency(true), m_transparencyGridType(GRID_LIGHT),
      m_uvMapProgram(0), m_gridProgram(0),
      m_displayMode(MODE_UV)
{
}

GLWidget::~GLWidget()
{
    m_uvMapProgram = 0;
    m_gridProgram = 0;
}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
    return QSize(200, 200);
}

void GLWidget::initializeGL()
{
    initializeGLFunctions();
    makeObject();

    // opengl settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_2D);

    // transform matrices
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(-1.0, +1.0, +1.0, -1.0, 4.0, 15.0);
    glMatrixMode(GL_MODELVIEW);

    // shaders
    QGLShader *vshader = new QGLShader(QGLShader::Vertex, this);
    vshader->compileSourceFile(":/glsl/passThru_vert.glsl");

    QGLShader *fshader = new QGLShader(QGLShader::Fragment, this);
    fshader->compileSourceFile(":/glsl/uvMap_frag.glsl");

    m_uvMapProgram = new QGLShaderProgram(this);
    m_uvMapProgram->addShader(vshader);
    m_uvMapProgram->addShader(fshader);
    m_uvMapProgram->link();

    fshader = new QGLShader(QGLShader::Fragment, this);
    fshader->compileSourceFile(":/glsl/transparencyGrid_frag.glsl");

    m_gridProgram = new QGLShaderProgram(this);
    m_gridProgram->addShader(vshader);
    m_gridProgram->addShader(fshader);
    m_gridProgram->link();

    emit initialized();
}

void GLWidget::paintGL()
{
    qglClearColor(Qt::black);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -10.0f);

    glVertexPointer(3, GL_FLOAT, 0, m_vertices.constData());
    glTexCoordPointer(2, GL_FLOAT, 0, m_texCoords.constData());
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH);
    glDepthFunc(GL_ALWAYS);

    if(m_transparencyGridType != GRID_NONE) {
        glViewport(0, 0, m_widgetSize.width(), m_widgetSize.height());

        m_gridProgram->bind();
        m_gridProgram->setUniformValue("windowSize", QVector2D((float)m_widgetSize.width(), (float)m_widgetSize.height()));
        m_gridProgram->setUniformValue("gridSize", 16.0f);
        switch(m_transparencyGridType) {
        case GRID_LIGHT:
            m_gridProgram->setUniformValue("colorA",QVector3D(1.0,1.0,1.0));
            m_gridProgram->setUniformValue("colorB",QVector3D(0.8,0.8,0.8));
            break;
        case GRID_DARK:
            m_gridProgram->setUniformValue("colorA",QVector3D(0.2,0.2,0.2));
            m_gridProgram->setUniformValue("colorB",QVector3D(0.4,0.4,0.4));
            break;
        }

        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        m_gridProgram->release();
    }

    glViewport(m_viewport.left(), m_viewport.top(), m_viewport.width(), m_viewport.height());

    switch(m_displayMode) {
    case MODE_UV:
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_mapTexture);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_displayTexture);

        m_uvMapProgram->bind();
        m_uvMapProgram->setUniformValue("mapTex", 0);
        m_uvMapProgram->setUniformValue("displayTex", 1);
        m_uvMapProgram->setUniformValue("transparency", m_showTransparency);
        break;
    case MODE_RAW:
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_rawTexture);
    }

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    if(m_displayMode == MODE_UV) {
        m_uvMapProgram->release();
    }

    glDisable(GL_BLEND);
    glDisable(GL_DEPTH);
}

void GLWidget::resizeGL(int width, int height)
{
    m_widgetSize = QSize(width,height);
    setViewport();
}

void GLWidget::setViewport()
{
    QSize textureSize = (m_displayMode == MODE_UV)?m_mapSize:m_rawSize;

    double aspectRatio = (double)textureSize.width()/(double)textureSize.height();

    int side;
    double factor;
    if(aspectRatio >= (double)m_widgetSize.width() / (double)m_widgetSize.height()) {
        side = (int)((double)m_widgetSize.width()/aspectRatio);
        m_viewport = QRect(0, (m_widgetSize.height() - side)/2, m_widgetSize.width(), side);
        factor = (double)textureSize.width()/(double)m_widgetSize.width();
    } else {
        side = (int)((double)m_widgetSize.height()*aspectRatio);
        m_viewport = QRect((m_widgetSize.width() - side)/2, 0, side, m_widgetSize.height());
        factor = (double)textureSize.height()/(double)m_widgetSize.height();
    }
    factor*=m_zoomFactor;

    if(factor != 0.0) {
        QSize size = m_viewport.size() * factor;
        m_viewport.translate(QPoint((m_viewport.width()-size.width())/2, (m_viewport.height()-size.height())/2));
        m_viewport.setSize(size);
    }
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    m_lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    //int dx = event->x() - lastPos.x();
    //int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        // handle dragging
    }
    //m_lastPos = event->pos();
}

void GLWidget::mouseReleaseEvent(QMouseEvent * /* event */)
{
    emit clicked();
}

void GLWidget::makeObject()
{
    m_texCoords.clear();
    m_vertices.clear();

    for (int j = 0; j < 4; ++j) {
        m_texCoords.append
            (QVector2D(j == 0 || j == 3, j == 2 || j == 3));
        m_vertices.append
            (QVector3D(
                 ((j == 0 || j == 3)?1:-1),
                 ((j == 2 || j == 3)?1:-1),
                 0.0)
            );
    }
}

void GLWidget::setMapTexture(GLuint texture)
{
    GLint width, height;
    glBindTexture(GL_TEXTURE_2D, texture);
    glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_WIDTH,&width);
    glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_HEIGHT,&height);

    m_mapTexture = texture;
    m_mapSize = QSize(width, height);

    setViewport();
    repaint();
}

void GLWidget::setDisplayTexture(GLuint texture)
{
    m_displayTexture = texture;
    repaint();
}

void GLWidget::setShowTransparency(bool show)
{
    m_showTransparency = show;
    repaint();
}

void GLWidget::setTransparencyGrid(TRANSPARENCYGRID_TYPE type)
{
    m_transparencyGridType = type;
    repaint();
}

void GLWidget::setMode(DISPLAY_MODE mode)
{
    m_displayMode = mode;

    setViewport();
    paintGL();
}

void GLWidget::setRawTexture(GLuint texture)
{
    GLint width, height;
    glBindTexture(GL_TEXTURE_2D, texture);
    glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_WIDTH,&width);
    glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_HEIGHT,&height);

    m_rawTexture = texture;
    m_rawSize = QSize(width, height);

    setMode(MODE_RAW);
}

void GLWidget::setZoom(double zoom)
{
    m_zoomFactor = zoom;
    setViewport();

    repaint();
}

void GLWidget::zoomInOut(bool in)
{
    if(m_zoomFactor == 0) {
        m_zoomFactor = 1;
    }
    m_zoomFactor *= (in)?1.25:0.8;
    setViewport();

    repaint();
}


