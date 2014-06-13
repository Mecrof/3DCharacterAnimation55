#include "Texture.hpp"
#include <iostream>

Texture::Texture(GLenum textureTarget, const char* fileName)
{
    m_TextureTarget = textureTarget;
    m_FileName      = fileName;
    m_pImage        = NULL;
}

bool Texture::load()
{
    m_pImage = new QPixmap(m_FileName);

    glGenTextures(1, &m_TextureObj);
    glBindTexture(m_TextureTarget, m_TextureObj);
    glTexImage2D(m_TextureTarget, 0, GL_RGBA, m_pImage->width(), m_pImage->height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, m_pImage->toImage().bits());
    glTexParameterf(m_TextureTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(m_TextureTarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    return true;
}

void Texture::bind(GLenum textureUnit)
{
    glActiveTexture(textureUnit);
    glBindTexture(m_TextureTarget, m_TextureObj);
}
