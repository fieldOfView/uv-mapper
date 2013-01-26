#include "mapmanager.h"

MapManager::MapManager()
{
    glGenTextures( 1, &texture );
}

MapManager::~MapManager()
{
    glDeleteTextures( 1, &texture );
}


void MapManager::setMat( cv::Mat newMat )
{
    map = newMat.clone();

    addHistoryState();
    updateTexture();
}

cv::Mat MapManager::getMat()
{
    return map;
}

QSize MapManager::getSize()
{
    return QSize(map.cols, map.rows);
}

GLuint MapManager::getTexture()
{
    return texture;
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

    fileName.clear();
    resetHistory();

    setMat(newMat);
}

bool MapManager::load( QString newFileName )
{
    if( newFileName.isNull() ) {
        newFileName = fileName;
        if( newFileName.isNull() ) {
            return false;
        }
    }

    cv::Mat loadedMat = cv::imread( newFileName.toStdString(), CV_LOAD_IMAGE_UNCHANGED );
    if( !loadedMat.data ) {
        qDebug() << "Could not load map from '" << newFileName << "'.";
        return false;
    }

    resetHistory();
    map = loadedMat;

    addHistoryState();
    updateTexture();

    fileName = newFileName;
    return true;
}

bool MapManager::save( QString newFileName )
{
    if( newFileName.isNull() ) {
        newFileName = fileName;
        if( newFileName.isNull() ) {
            return false;
        }
    }

    if( !cv::imwrite( newFileName.toStdString(), map )) {
        qDebug() << "Could not save map to '" << newFileName << "'.";
        return false;
    }

    fileName = newFileName;
    return true;
}

bool MapManager::undo()
{
    if( historyIndex == 0 ) {
        return false;
    }

    historyIndex--;
    map = history.at(historyIndex);

    updateTexture();

    if( historyIndex == 0 ) {
        // no more undos available
        return false;
    }

    return true;
}

bool MapManager::redo()
{
    if( historyIndex == history.count()-1 ) {
        return false;
    }

    historyIndex++;
    map = history.at(historyIndex);

    updateTexture();

    if( historyIndex == history.count()-1 ) {
        // no more redos available
        return false;
    }

    return true;
}

void MapManager::addHistoryState()
{
    if( historyIndex < (history.count()-1) )
        resetHistory( historyIndex );

    history.push_back( map );
    historyIndex = history.count()-1;
}

void MapManager::resetHistory( int fromIndex )
{
    QVectorIterator<cv::Mat> historyItr(history);
    historyItr.toBack();
    while( historyItr.hasPrevious() && history.count() > fromIndex ) {
        cv::Mat mat = historyItr.previous();
        mat.release();
        history.pop_back();
    }
}

void MapManager::updateTexture()
{
    GLenum type, format;

    switch (map.depth()) {
    case CV_8U:
        format = GL_UNSIGNED_BYTE;
        break;
    case CV_16U:
        format = GL_UNSIGNED_SHORT;
        break;
    }

    switch (map.channels()) {
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

    glTexImage2D( GL_TEXTURE_2D, 0, glFormat, map.cols, map.rows, 0, type, format, map.data);
}
