#include "Spatial.hpp"

namespace scene
{
    Spatial::Spatial()
    {
        m_position = glm::vec3(0.0f);
        m_rotation = glm::angleAxis(0.0f, glm::vec3(0.0f));
        m_scale = glm::vec3(1.0f);
    }

    glm::vec3 *Spatial::translate(glm::vec3 vector)
    {
        m_position += vector;
        return &m_position;
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
        m_rotation *= rotation;
        return &m_rotation;
    }

    glm::vec3 *Spatial::scale(glm::vec3 factors)
    {
        m_scale *= factors;
        return &m_scale;
    }

    glm::vec3 *Spatial::scale(float x, float y, float z)
    {
        return scale(glm::vec3(x,y,z));
    }

    glm::vec3 &Spatial::getPosition()
    {
        return m_position;
    }

    void Spatial::setPosition(glm::vec3 position)
    {
        m_position = position;
    }

    void Spatial::setPosition(float x, float y, float z)
    {
        m_position.x = x; m_position.y = y; m_position.z = z;
    }

    glm::quat &Spatial::getRotation()
    {
        return m_rotation;
    }

    void Spatial::setRotation(glm::quat rotation)
    {
        m_rotation = rotation;
    }

    void Spatial::setRotation(float angle, glm::vec3 axis)
    {
        m_rotation = glm::angleAxis(angle, axis);
    }

    glm::vec3 &Spatial::getScale()
    {
        return m_scale;
    }

    void Spatial::setScale(glm::vec3 scale)
    {
        m_scale = scale;
    }

    void Spatial::setScale(float x, float y, float z)
    {
        m_scale.x = x; m_scale.y = y; m_scale.z = z;
    }

}
