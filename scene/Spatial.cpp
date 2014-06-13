#include "Spatial.hpp"

namespace scene
{
    Spatial::Spatial()
    {
        m_Position = glm::vec3(0.0f);
        m_Rotation = glm::angleAxis(0.0f, glm::vec3(0.0f));
        m_Scale = glm::vec3(1.0f);
    }

    glm::vec3 *Spatial::translate(glm::vec3 vector)
    {
        m_Position += vector;
        return &m_Position;
    }

    glm::vec3 *Spatial::translate(float x, float y, float z)
    {
        return translate(glm::vec3(x,y,z));
    }

    glm::quat *Spatial::rotate(float angle, glm::vec3 axis)
    {
        return rotate(glm::angleAxis(angle, axis));
    }

    glm::quat *Spatial::rotate(glm::quat rotation)
    {
        m_Rotation *= rotation;
        return &m_Rotation;
    }

    glm::vec3 *Spatial::scale(glm::vec3 factors)
    {
        m_Scale *= factors;
        return &m_Scale;
    }

    glm::vec3 *Spatial::scale(float x, float y, float z)
    {
        return scale(glm::vec3(x,y,z));
    }

    glm::vec3 &Spatial::getPosition()
    {
        return m_Position;
    }

    void Spatial::setPosition(glm::vec3 position)
    {
        m_Position = position;
    }

    void Spatial::setPosition(float x, float y, float z)
    {
        m_Position.x = x; m_Position.y = y; m_Position.z = z;
    }

    glm::quat &Spatial::getRotation()
    {
        return m_Rotation;
    }

    void Spatial::setRotation(glm::quat rotation)
    {
        m_Rotation = rotation;
    }

    void Spatial::setRotation(float angle, glm::vec3 axis)
    {
        m_Rotation = glm::angleAxis(angle, axis);
    }

    glm::vec3 &Spatial::getScale()
    {
        return m_Scale;
    }

    void Spatial::setScale(glm::vec3 scale)
    {
        m_Scale = scale;
    }

    void Spatial::setScale(float x, float y, float z)
    {
        m_Scale.x = x; m_Scale.y = y; m_Scale.z = z;
    }

}
