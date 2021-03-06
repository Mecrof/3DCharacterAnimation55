#ifndef CAMERA_HPP
#define CAMERA_HPP

// GLM includes
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// Mouse event include
#include <QMouseEvent>
// Keyboard event include
#include <QKeyEvent>

#include <iostream>

namespace scene
{
    class Camera
    {
    public:
        Camera();
        ~Camera();
        Camera(glm::vec3 position, glm::vec3 cible, glm::vec3 axeVertical);
        void orientation(int xRel, int yRel, float m_Sensivity);
        void move(QEvent *event);
        void lookat(glm::mat4 &modelview);

        void setTarget(glm::vec3 pointCible);
        void setPosition(glm::vec3 position);

        void setSensivity(float m_Sensivity);


    private:
        float m_Phi;
        float m_Theta;
        glm::vec3 m_Orientation;
        glm::vec3 m_Verticalaxis;
        glm::vec3 m_SideMove;
        glm::vec3 m_Position;
        glm::vec3 m_Target;
        float m_Sensivity;
    };
}
#endif // CAMERA_HPP
