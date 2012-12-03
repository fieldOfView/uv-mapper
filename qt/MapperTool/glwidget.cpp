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
      aspectRatio(1),
      transparencyGridType(GRID_LIGHT),
      xRot(0),yRot(0),zRot(0),
      uvMapProgram(0), gridProgram(0)
{
}

GLWidget::~GLWidget()
{
    uvMapProgram = 0;
    gridProgram = 0;
}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
    return QSize(200, 200);
}

void GLWidget::rotateBy(int xAngle, int yAngle, int zAngle)
{
    xRot += xAngle;
    yRot += yAngle;
    zRot += zAngle;
    updateGL();
}

void GLWidget::initializeGL()
{
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

    uvMapProgram = new QGLShaderProgram(this);
    uvMapProgram->addShader(vshader);
    uvMapProgram->addShader(fshader);
    uvMapProgram->link();

    fshader = new QGLShader(QGLShader::Fragment, this);
    fshader->compileSourceFile(":/glsl/transparencyGrid_frag.glsl");

    gridProgram = new QGLShaderProgram(this);
    gridProgram->addShader(vshader);
    gridProgram->addShader(fshader);
    gridProgram->link();
}

void GLWidget::paintGL()
{
    qglClearColor(Qt::black);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -10.0f);
    glRotatef(xRot / 16.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(yRot / 16.0f, 0.0f, 1.0f, 0.0f);
    glRotatef(zRot / 16.0f, 0.0f, 0.0f, 1.0f);

    glVertexPointer(3, GL_FLOAT, 0, vertices.constData());
    glTexCoordPointer(2, GL_FLOAT, 0, texCoords.constData());
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH);
    glDepthFunc(GL_ALWAYS);

    if(transparencyGridType != GRID_NONE) {
        glViewport(0, 0, widgetWidth, widgetHeight);

        gridProgram->bind();
        gridProgram->setUniformValue("windowSize", QVector2D((float)widgetWidth, (float)widgetHeight));
        gridProgram->setUniformValue("gridSize", 16.0f);
        switch(transparencyGridType) {
        case GRID_LIGHT:
            gridProgram->setUniformValue("colorA",QVector3D(1.0,1.0,1.0));
            gridProgram->setUniformValue("colorB",QVector3D(0.8,0.8,0.8));
            break;
        case GRID_DARK:
            gridProgram->setUniformValue("colorA",QVector3D(0.2,0.2,0.2));
            gridProgram->setUniformValue("colorB",QVector3D(0.4,0.4,0.4));
            break;
        }

        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        gridProgram->release();
    }

    glViewport(viewport.left(), viewport.top(), viewport.width(), viewport.height());

    uvMapProgram->bind();
    uvMapProgram->setUniformValue("texture", 0);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    uvMapProgram->release();

    glDisable(GL_BLEND);
    glDisable(GL_DEPTH);
}

void GLWidget::resizeGL(int width, int height)
{
    widgetWidth = width;
    widgetHeight = height;
    setViewport();
}

void GLWidget::setViewport()
{
    int side;
    if(aspectRatio >= (double)widgetWidth / (double)widgetHeight) {
        side = (int)((double)widgetWidth/aspectRatio);
        viewport = QRect(0, (widgetHeight - side)/2, widgetWidth, side);
    } else {
        side = (int)((double)widgetHeight*aspectRatio);
        viewport = QRect((widgetWidth - side)/2, 0, side, widgetHeight);
    }

    float factor = 1.0f;
    if(factor != 1.0f) {
        QSize size(viewport.width(),viewport.height());
        size *= factor;
        QPoint offset((viewport.width()-size.width())/2, (viewport.height()-size.height())/2);
        viewport.translate(offset);
        viewport.setSize(size);
    }
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        rotateBy(8 * dy, 8 * dx, 0);
    } else if (event->buttons() & Qt::RightButton) {
        rotateBy(8 * dy, 0, 8 * dx);
    }
    lastPos = event->pos();
}

void GLWidget::mouseReleaseEvent(QMouseEvent * /* event */)
{
    emit clicked();
}

void GLWidget::makeObject()
{
    texCoords.clear();
    vertices.clear();

    for (int j = 0; j < 4; ++j) {
        texCoords.append
            (QVector2D(j == 0 || j == 3, j == 2 || j == 3));
        vertices.append
            (QVector3D(
                 ((j == 0 || j == 3)?1:-1),
                 ((j == 2 || j == 3)?1:-1),
                 0.0)
            );
    }
}

void GLWidget::setTexture(GLuint texture)
{
    mapTexture = texture;
    repaint();
}

void GLWidget::setAspectRatio(double ratio)
{
    if(aspectRatio != ratio) {
        aspectRatio = ratio;
        setViewport();
    }
}

void GLWidget::setTransparencyGrid(TRANSPARENCYGRID_TYPE type)
{
    transparencyGridType = type;
    repaint();
}

