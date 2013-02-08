#ifndef TEXTUREFROMMAT_H
#define TEXTUREFROMMAT_H

#include <QtOpenGL>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

GLuint makeTextureFromMat(cv::Mat mat, GLuint texture)
{
    GLenum type, format;

    switch (mat.depth()) {
    case CV_8U:
        format = GL_UNSIGNED_BYTE;
        break;
    case CV_16U:
        format = GL_UNSIGNED_SHORT;
        break;
    }

    switch (mat.channels()) {
    case 3:
        type = GL_BGR_EXT;
        break;
    case 4:
        type = GL_BGRA_EXT;
        break;
    }

    GLint glFormat = (type == GL_BGR_EXT)?
                ((format == GL_UNSIGNED_BYTE)?GL_RGB8:GL_RGB16):
                ((format == GL_UNSIGNED_BYTE)?GL_RGBA8:GL_RGBA16);

    glBindTexture( GL_TEXTURE_2D, texture );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

    glTexImage2D( GL_TEXTURE_2D, 0, glFormat, mat.cols, mat.rows, 0, type, format, mat.data);

    return texture;
}



#endif // TEXTUREFROMMAT_H
