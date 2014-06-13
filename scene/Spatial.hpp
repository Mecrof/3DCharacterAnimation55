#ifndef SPATIAL_H
#define SPATIAL_H

// GLM includes
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace scene
{
    class Spatial
    {
    public:
        Spatial();

        glm::vec3 *translate(glm::vec3 vector);
        glm::vec3 *translate(float x, float y, float z);
        glm::quat *rotate(float angle, glm::vec3 axis);
        glm::quat *rotate(glm::quat rotation);
        glm::vec3 *scale(glm::vec3 factors);
        glm::vec3 *scale(float x, float y, float z);

        glm::vec3 &getPosition();
        void setPosition(glm::vec3 position);
        void setPosition(float x, float y, float z);
        glm::quat &getRotation();
        void setRotation(glm::quat rotation);
        void setRotation(float angle, glm::vec3 axis);
        glm::vec3 &getScale();
        void setScale(glm::vec3 scale);
        void setScale(float x, float y, float z);

    protected:
        glm::vec3 m_Position;
        glm::quat m_Rotation;
        glm::vec3 m_Scale;


    };
}

#endif // SPATIAL_H
