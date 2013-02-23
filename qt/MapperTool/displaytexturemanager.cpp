#include "displaytexturemanager.h"

DisplayTextureManager::DisplayTextureManager()
    : m_generatedImage(0),
      m_texture(0),
      m_displayType(DISPLAY_TYPE_NULL)
{
}

DisplayTextureManager::~DisplayTextureManager()
{
}

GLuint DisplayTextureManager::getTexture()
{
    return m_texture;
}

void DisplayTextureManager::makeTexture( DISPLAY_TYPE type )
{
    if(m_displayType == type)
        return;

    m_displayType = type;
    if(m_generatedImage){
        delete m_generatedImage;
    }

    switch(m_displayType) {
    case DISPLAY_TYPE_UV:
        m_generatedImage = new GeneratedImage(QSize(256, 256));
        m_generatedImage->drawGradient(QColor("#000000"),QColor("#ff0000"),QColor("#00ff00"),QColor("#ffff00"));
        break;
    case DISPLAY_TYPE_U:
        m_generatedImage = new GeneratedImage(QSize(256, 2));
        m_generatedImage->drawGradient(QColor("#000000"),QColor("#ffffff"),QColor("#000000"),QColor("#ffffff"));
        break;
    case DISPLAY_TYPE_V:
        m_generatedImage = new GeneratedImage(QSize(2, 256));
        m_generatedImage->drawGradient(QColor("#000000"),QColor("#000000"),QColor("#ffffff"),QColor("#ffffff"));
        break;
    case DISPLAY_TYPE_A:
        m_generatedImage = new GeneratedImage(QSize(2, 2));
        m_generatedImage->drawGradient(QColor("#ffffff"),QColor("#ffffff"),QColor("#ffffff"),QColor("#ffffff"));
        break;
    case DISPLAY_TYPE_GRID:
        m_generatedImage = new GeneratedImage(QSize(1024, 1024));
        m_generatedImage->drawGrid(QColor("#000000"),QColor("#ffffff"),32,(float)1/8);
        break;
    case DISPLAY_TYPE_FILE:
        // TODO
        //m_generatedImage = new GeneratedImage(QSize(1024, 1024));
        break;
    }
    if(m_generatedImage) {
        m_texture = m_generatedImage->getTexture();
    }
}

bool DisplayTextureManager::load( QString fileName )
{
    qDebug() << "Open display file: " << fileName << endl;
    return true;
}
