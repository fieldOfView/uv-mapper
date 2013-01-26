#include "displaytexturemanager.h"

DisplayTextureManager::DisplayTextureManager()
{

}

DisplayTextureManager::~DisplayTextureManager()
{

}

GLuint DisplayTextureManager::getTexture()
{
    return texture;
}

void DisplayTextureManager::makeTexture( DISPLAY_TYPE type )
{
    switch(type) {
    case DISPLAY_TYPE_UV:
        break;
    case DISPLAY_TYPE_U:
        break;
    case DISPLAY_TYPE_V:
        break;
    case DISPLAY_TYPE_A:
        break;
    case DISPLAY_TYPE_GRID:
        break;
    case DISPLAY_TYPE_FILE:
        break;
    }
}

bool DisplayTextureManager::load( QString fileName )
{
    qDebug() << "Open display file: " << fileName << endl;
    return true;
}
