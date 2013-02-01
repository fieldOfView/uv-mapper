#include "patternmanager.h"
#include <QVectorIterator>

PatternManager::PatternManager()
{
    m_fileNames.push_back("/home/arnaud/Downloads/1 - raw photos/_DSC3899.tif");
    m_fileNames.push_back("/home/arnaud/Downloads/1 - raw photos/_DSC3890.tif");
    m_fileNames.push_back("/home/arnaud/Downloads/1 - raw photos/_DSC3888.tif");
    m_fileNames.push_back("/home/arnaud/Downloads/1 - raw photos/_DSC3904.tif");
    m_fileNames.push_back("/home/arnaud/Downloads/1 - raw photos/_DSC3901.tif");
    m_fileNames.push_back("/home/arnaud/Downloads/1 - raw photos/_DSC3884.tif");
    m_fileNames.push_back("/home/arnaud/Downloads/1 - raw photos/_DSC3885.tif");
    m_fileNames.push_back("/home/arnaud/Downloads/1 - raw photos/_DSC3896.tif");
    m_fileNames.push_back("/home/arnaud/Downloads/1 - raw photos/_DSC3882.tif");
    m_fileNames.push_back("/home/arnaud/Downloads/1 - raw photos/_DSC3903.tif");
    m_fileNames.push_back("/home/arnaud/Downloads/1 - raw photoglTexCoord2fs/_DSC3887.tif");
    m_fileNames.push_back("/home/arnaud/Downloads/1 - raw photos/_DSC3898.tif");
    m_fileNames.push_back("/home/arnaud/Downloads/1 - raw photos/_DSC3894.tif");
    m_fileNames.push_back("/home/arnaud/Downloads/1 - raw photos/_DSC3891.tif");
    m_fileNames.push_back("/home/arnaud/Downloads/1 - raw photos/_DSC3886.tif");
    m_fileNames.push_back("/home/arnaud/Downloads/1 - raw photos/_DSC3893.tif");
    m_fileNames.push_back("/home/arnaud/Downloads/1 - raw photos/_DSC3905.tif");
    m_fileNames.push_back("/home/arnaud/Downloads/1 - raw photos/_DSC3897.tif");
    m_fileNames.push_back("/home/arnaud/Downloads/1 - raw photos/_DSC3895.tif");
    m_fileNames.push_back("/home/arnaud/Downloads/1 - raw photos/_DSC3892.tif");
    m_fileNames.push_back("/home/arnaud/Downloads/1 - raw photos/_DSC3902.tif");
    m_fileNames.push_back("/home/arnaud/Downloads/1 - raw photos/_DSC3889.tif");
    m_fileNames.push_back("/home/arnaud/Downloads/1 - raw photos/_DSC3900.tif");
    m_fileNames.push_back("/home/arnaud/Downloads/1 - raw photos/_DSC3883.tif");

    QVectorIterator<QString> it(m_fileNames);
    while(it.hasNext()) {

        m_filesToLoad.enqueue(it.next().toStdString());
    }

    for(int i=0;i<m_fileNames.size();i++) {
        qt_grayDecoder* decoder = new qt_grayDecoder(m_filesToLoad);
        m_decoders.push_back(decoder);
        connect(decoder, SIGNAL(fileLoaded(cv::Mat*)), this, SLOT(fileLoaded(cv::Mat*)));
        decoder->start();
    }
}

bool PatternManager::loadFiles(QString fileName) {
    return true;
}

void PatternManager::fileLoaded(cv::Mat *file) {

    this->m_originalPatterns.push_back(file);
    qDebug("file loaded");
}

GLuint PatternManager::getTexture(int index)
{
    /*
     * convert cv::Mat to openGL texture
     */
    GLuint texture;
    GLenum type, format;

    cv::Mat* map = m_originalPatterns.at(index);

    switch (map->depth()) {
    case CV_8U:
        format = GL_UNSIGNED_BYTE;
        break;
    case CV_16U:
        format = GL_UNSIGNED_SHORT;
        break;
    }

    switch (map->channels()) {
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
    glTexImage2D( GL_TEXTURE_2D, 0, glFormat, map->cols, map->rows, 0, type, format, map->data);
    return texture;
}
