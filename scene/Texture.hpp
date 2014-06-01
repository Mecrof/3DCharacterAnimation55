#ifndef TEXTURE_H
#define TEXTURE_H


#include <QPixmap>
#include <GL/glew.h>

class Texture
{
    public:
        Texture(GLenum textureTarget, const char* fileName);

        bool load();

        void bind(GLenum textureUnit);

    private:
        const char* m_fileName;
        GLenum m_textureTarget;
        GLuint m_textureObj;
        QPixmap* m_pImage;
};

#endif // TEXTURE_H
