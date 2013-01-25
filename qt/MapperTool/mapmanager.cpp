#include "mapmanager.h"

MapManager::MapManager()
{
    glGenTextures( 1, &m_texture );
}

MapManager::~MapManager()
{
    glDeleteTextures( 1, &m_texture );
}


void MapManager::setMat( cv::Mat newMat )
{
    m_map = newMat.clone();

    addHistoryState();
    updateTexture();
}

cv::Mat MapManager::getMat()
{
    return m_map;
}

QSize MapManager::getSize()
{
    return QSize(m_map.cols, m_map.rows);
}

GLuint MapManager::getTexture()
{
    return m_texture;
}

bool MapManager::load( QString fileName )
{
    if( fileName.isNull() ) {
        fileName = m_fileName;
        if( fileName.isNull() ) {
            return false;
        }
    }

    cv::Mat loadedMat = cv::imread( fileName.toStdString(), CV_LOAD_IMAGE_UNCHANGED );
    if( !loadedMat.data ) {
        qDebug() << "Could not load map from '" << fileName << "'.";
        return false;
    }

    resetHistory();
    m_map = loadedMat;

    addHistoryState();
    updateTexture();

    m_fileName = fileName;
    return true;
}

bool MapManager::save( QString fileName )
{
    if( fileName.isNull() ) {
        fileName = m_fileName;
        if( fileName.isNull() ) {
            return false;
        }
    }

    if( !cv::imwrite( fileName.toStdString(), m_map )) {
        qDebug() << "Could not save map to '" << fileName << "'.";
        return false;
    }

    m_fileName = fileName;
    return true;
}

bool MapManager::undo()
{
    if( m_historyIndex == 0 ) {
        return false;
    }

    m_historyIndex--;
    m_map = m_history.at(m_historyIndex);

    updateTexture();

    if( m_historyIndex == 0 ) {
        // no more undos available
        return false;
    }

    return true;
}

bool MapManager::redo()
{
    if( m_historyIndex == m_history.count()-1 ) {
        return false;
    }

    m_historyIndex++;
    m_map = m_history.at(m_historyIndex);

    updateTexture();

    if( m_historyIndex == m_history.count()-1 ) {
        // no more redos available
        return false;
    }

    return true;
}

void MapManager::addHistoryState()
{
    if( m_historyIndex < (m_history.count()-1) )
        resetHistory( m_historyIndex );

    m_history.push_back( m_map );
    m_historyIndex = m_history.count()-1;
}

void MapManager::resetHistory( int fromIndex )
{
    QVectorIterator<cv::Mat> historyItr(m_history);
    historyItr.toBack();
    while( historyItr.hasPrevious() && m_history.count() > fromIndex ) {
        cv::Mat mat = historyItr.previous();
        mat.release();
        m_history.pop_back();
    }
}

void MapManager::updateTexture()
{
    GLenum type, format;

    switch (m_map.depth()) {
    case CV_8U:
        format = GL_UNSIGNED_BYTE;
        break;
    case CV_16U:
        format = GL_UNSIGNED_SHORT;
        break;
    }

    switch (m_map.channels()) {
    case 3:
        type = GL_BGR_EXT;
        break;
    case 4:
        type = GL_BGRA_EXT;
        break;
    }

    glBindTexture( GL_TEXTURE_2D, m_texture );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA16, m_map.cols, m_map.rows, 0, type, format, m_map.data);
}
