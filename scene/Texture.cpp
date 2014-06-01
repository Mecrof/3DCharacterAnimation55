#include "Texture.hpp"
#include <iostream>

Texture::Texture(GLenum textureTarget, const char* fileName)
{
    m_textureTarget = textureTarget;
    m_fileName      = fileName;
    m_pImage        = NULL;
}

bool Texture::load()
{
    m_pImage = new QPixmap(m_fileName);

    glGenTextures(1, &m_textureObj);
    glBindTexture(m_textureTarget, m_textureObj);
    glTexImage2D(m_textureTarget, 0, GL_RGBA, m_pImage->width(), m_pImage->height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, m_pImage->toImage().bits());
    glTexParameterf(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    return true;
}

void Texture::bind(GLenum textureUnit)
{
    glActiveTexture(textureUnit);
    glBindTexture(m_textureTarget, m_textureObj);
}
