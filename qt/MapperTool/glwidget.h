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

 #ifndef GLWIDGET_H
 #define GLWIDGET_H

 #include <QtGui>
 #include <QGLWidget>

 class QGLShaderProgram;

 class GLWidget : public QGLWidget
 {
     Q_OBJECT

 public:
     GLWidget(QWidget *parent = 0, QGLWidget *shareWidget = 0);
     ~GLWidget();

     QSize minimumSizeHint() const;
     QSize sizeHint() const;

     enum TRANSPARENCYGRID_TYPE {
       GRID_NONE,
       GRID_LIGHT,
       GRID_DARK
     };

     void setTransparencyGrid(TRANSPARENCYGRID_TYPE type);
     void setMapTexture(GLuint texture);
     void setDisplayTexture(GLuint texture);

     void setZoom(double zoom);
     void zoomInOut(bool in);

 signals:
     void clicked();

 protected:
     void initializeGL();
     void paintGL();
     void resizeGL(int width, int height);

     void mousePressEvent(QMouseEvent *event);
     void mouseMoveEvent(QMouseEvent *event);
     void mouseReleaseEvent(QMouseEvent *event);

 private:
     void makeObject();
     void setViewport();

     GLuint mapTexture;
     QSize mapSize;
     GLuint displayTexture;

     QVector<QVector3D> vertices;
     QVector<QVector2D> texCoords;

     QSize widgetSize;
     QRect viewport;
     double zoomFactor;

     TRANSPARENCYGRID_TYPE transparencyGridType;
     QPoint lastPos;

     QGLShaderProgram *uvMapProgram;
     QGLShaderProgram *gridProgram;
 };

 #endif
