#include "mapmanager.h"
#include "texturefrommat.h"

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

void MapManager::updateTexture()
{
    makeTextureFromMat(m_map, m_texture);
}

void MapManager::createFromTexture( GLuint texture )
{
    GLint width, height, glFormat;
    int depth=0, channels=0, type;

    glBindTexture(GL_TEXTURE_2D, texture);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &glFormat);

    switch(glFormat) {
    case GL_RGB8:
        depth = CV_8U;
        channels = 3;
        break;
    case GL_RGBA8:
        depth = CV_8U;
        channels = 4;
        break;
    case GL_RGB16:
        depth = CV_16U;
        channels = 3;
        break;
    case GL_RGBA16:
        depth = CV_16U;
        channels = 4;
        break;
    default:
        qDebug() << "Unsupported texture format";
        return;
        break;
    }

    type = (depth == CV_8U)?
                ((channels == 3)?CV_8UC3:CV_8UC4):
                ((channels == 3)?CV_16UC3:CV_16UC4);
    cv::Mat newMat(height, width, type);

    glGetTexImage(GL_TEXTURE_2D,0,
                 (channels == 3)?GL_BGR_EXT:GL_BGRA_EXT,
                 (depth == CV_8U)?GL_UNSIGNED_BYTE:GL_UNSIGNED_SHORT,
                 newMat.data);

    m_fileName.clear();
    resetHistory();

    setMat(newMat);
}

bool MapManager::load( QString newFileName )
{
    if( newFileName.isEmpty() ) {
        newFileName = m_fileName;
        if( newFileName.isEmpty() ) {
            return false;
        }
    }

    QMessageBox msgBox;
    msgBox.setText("This document does not seem to be a valid UV-map.");
    msgBox.setIcon(QMessageBox::Critical);

    cv::Mat loadedMat = cv::imread( newFileName.toStdString(), CV_LOAD_IMAGE_UNCHANGED );
    if( !loadedMat.data ) {
        msgBox.setInformativeText("The document is not an image or could not be read.");
        msgBox.exec();
        return false;
    }

    if( loadedMat.depth() != CV_16U ) {
        msgBox.setInformativeText("UV-map documents should be 16 bit per color channel.");
        msgBox.exec();
        return false;
    }

    resetHistory();
    m_map = loadedMat;

    addHistoryState();
    updateTexture();

    m_fileName = newFileName;
    return true;
}

bool MapManager::save( QString newFileName )
{
    if( newFileName.isEmpty() ) {
        newFileName = m_fileName;
        if( newFileName.isEmpty() ) {
            return false;
        }
    }

    if( !cv::imwrite( newFileName.toStdString(), m_map )) {
        qDebug() << "Could not save map to '" << newFileName << "'.";
        return false;
    }

    m_fileName = newFileName;
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
