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
        const char* m_FileName;
        GLenum m_TextureTarget;
        GLuint m_TextureObj;
        QPixmap* m_pImage;
};

#endif // TEXTURE_H
