#include "displaytexturemanager.h"

DisplayTextureManager::DisplayTextureManager()
    : generatedImage(0),
      texture(0),
      displayType(DISPLAY_TYPE_NULL)
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
    if(displayType == type)
        return;

    displayType = type;
    if(generatedImage){
        delete generatedImage;
    }

    switch(type) {
    case DISPLAY_TYPE_UV:
        generatedImage = new GeneratedImage(QSize(256, 256));
        generatedImage->drawGradient(QColor("#000000"),QColor("#ff0000"),QColor("#00ff00"),QColor("#ffff00"));
        break;
    case DISPLAY_TYPE_U:
        generatedImage = new GeneratedImage(QSize(256, 2));
        generatedImage->drawGradient(QColor("#000000"),QColor("#ffffff"),QColor("#000000"),QColor("#ffffff"));
        break;
    case DISPLAY_TYPE_V:
        generatedImage = new GeneratedImage(QSize(2, 256));
        generatedImage->drawGradient(QColor("#000000"),QColor("#000000"),QColor("#ffffff"),QColor("#ffffff"));
        break;
    case DISPLAY_TYPE_A:
        generatedImage = new GeneratedImage(QSize(2, 2));
        generatedImage->drawGradient(QColor("#ffffff"),QColor("#ffffff"),QColor("#ffffff"),QColor("#ffffff"));
        break;
    case DISPLAY_TYPE_GRID:
        // TODO
        generatedImage = new GeneratedImage(QSize(1024, 1024));
        //generatedImage->drawGrid();
        break;
    case DISPLAY_TYPE_FILE:
        // TODO
        //generatedImage = new GeneratedImage(QSize(1024, 1024));
        break;
    }
    if(generatedImage) {
        texture = generatedImage->getTexture();
    }
}

bool DisplayTextureManager::load( QString fileName )
{
    qDebug() << "Open display file: " << fileName << endl;
    return true;
}
