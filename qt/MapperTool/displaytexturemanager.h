#ifndef DISPLAYTEXTUREMANAGER_H
#define DISPLAYTEXTUREMANAGER_H

#include <QRect>
#include <QString>
#include <QtOpenGL>

class DisplayTextureManager
{
public:
    DisplayTextureManager();
    ~DisplayTextureManager();

    enum DISPLAY_TYPE {
        DISPLAY_TYPE_UV,
        DISPLAY_TYPE_U,
        DISPLAY_TYPE_V,
        DISPLAY_TYPE_A,
        DISPLAY_TYPE_GRID,
        DISPLAY_TYPE_FILE
    };

    GLuint getTexture();
    void makeTexture( DISPLAY_TYPE type );

    bool load( QString fileName );



private:
    GLuint m_texture;
};

#endif // DISPLAYTEXTUREMANAGER_H
