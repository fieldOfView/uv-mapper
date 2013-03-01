#include "texturefrommat.h"

GLuint makeTextureFromMat(cv::Mat& mat, GLuint texture)
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

    GLint glFormat;
    switch (mat.channels()) {
    case 1:
        type = GL_LUMINANCE;
        glFormat = (format == GL_UNSIGNED_BYTE)?GL_LUMINANCE8:GL_LUMINANCE16;
        break;
    case 3:
        type = GL_BGR_EXT;
        glFormat = (format == GL_UNSIGNED_BYTE)?GL_RGB8:GL_RGB16;
        break;
    case 4:
        type = GL_BGRA_EXT;
        glFormat = (format == GL_UNSIGNED_BYTE)?GL_RGBA8:GL_RGBA16;
        break;
    }

    glBindTexture( GL_TEXTURE_2D, texture );

    glPixelStorei( GL_PACK_ALIGNMENT, 1 );
    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

    glTexImage2D( GL_TEXTURE_2D, 0, glFormat, mat.cols, mat.rows, 0, type, format, mat.data);

    return texture;
}

