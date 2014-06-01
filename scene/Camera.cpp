#include "Camera.hpp"
namespace scene
{
    Camera::Camera()
    {
        m_phi = 0.0f;
        m_theta = 0.0f;
        m_orientation = glm::vec3(0.0f, 0.0f, 0.0f);
        m_Verticalaxis = glm::vec3(0.0f, 0.0f, 1.0f);
        m_sideMove = glm::vec3(0.0f, 0.0f, 0.0f);
        m_position = glm::vec3(0.0f, 0.0f, 0.0f);
        m_target = glm::vec3(0.0f, 0.0f, 0.0f);
        sensivity = 0.5f;
    }

    Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 verticalaxis)
    {
        m_phi = -35.26f;
        m_theta = -135.0f;
        m_orientation = glm::vec3(0.0f, 0.0f, 0.0f);
        m_Verticalaxis = verticalaxis;
        m_sideMove = glm::vec3(0.0f, 0.0f, 0.0f);
        m_position = position;
        m_target = target;
        sensivity = 0.5f;
    }

    Camera::~Camera()
    {

    }

    void Camera::orientation(int xRel, int yRel, float sensibility)
    {
        // "-=" for trigonometric angles
        m_phi   -= yRel*sensibility;
        m_theta -= xRel*sensibility;

        if(m_phi > 89.0f)
        {
            m_phi = 89.0f;
        }
        else if(m_phi < -89.0f)
        {
            m_phi = -89.0f;
        }
        /*
        else if(m_theta < -89.0f)
        {
            m_theta = -89.0f;
        }
        */

        float phiRadian = m_phi * M_PI / 180;
        float thetaRadian = m_theta * M_PI / 180;

        // If vertical axis is x
        if(m_Verticalaxis.x == 1.0)
        {
            m_orientation.x = sin(phiRadian);
            m_orientation.y = cos(phiRadian) * cos(thetaRadian);
            m_orientation.z = cos(phiRadian) * sin(thetaRadian);
        }


        // If vertical axis is y
        else if(m_Verticalaxis.y == 1.0)
        {
            m_orientation.x = cos(phiRadian) * sin(thetaRadian);
            m_orientation.y = sin(phiRadian);
            m_orientation.z = cos(phiRadian) * cos(thetaRadian);
        }


        // If vertical axis is z
        else
        {
            m_orientation.x = cos(phiRadian) * cos(thetaRadian);
            m_orientation.y = cos(phiRadian) * sin(thetaRadian);
            m_orientation.z = sin(phiRadian);
        }


        // Normal calcul
        m_sideMove = glm::cross(m_Verticalaxis, m_orientation);
        m_sideMove = glm::normalize(m_sideMove);

        // Target calcul
        m_target = m_position + m_orientation;
    }

    void Camera::move(QEvent *event)
    {
        if(event->type() == QKeyEvent::KeyPress)
        {
            QKeyEvent *key = static_cast<QKeyEvent *>(event);
            switch(key->key())
            {
                case Qt::Key_Up:
                    m_position = m_position + m_orientation * sensivity;
                    m_target = m_position + m_orientation;
                    break;
                case Qt::Key_Down:
                    m_position = m_position - m_orientation * sensivity;
                    m_target = m_position + m_orientation;
                    break;
                case Qt::Key_Left:
                    m_position = m_position + m_sideMove * sensivity;
                    m_target = m_position + m_orientation;
                    break;
                case Qt::Key_Right:
                    m_position = m_position - m_sideMove * sensivity;
                    m_target = m_position + m_orientation;
                    break;
            }
       }
    }

    void Camera::lookat(glm::mat4 &modelview)
    {
        modelview = glm::lookAt(m_position, m_target, m_Verticalaxis);
    }

    void Camera::setTarget(glm::vec3 target)
    {
        // Orientation vector calcul
        m_orientation = m_target - m_position;
        m_orientation = glm::normalize(m_orientation);


        // If vertical axis is x
        if(m_Verticalaxis.x == 1.0)
        {
            m_phi = asin(m_orientation.x);
            m_theta = acos(m_orientation.y / cos(m_phi));

            if(m_orientation.y < 0)
                m_theta *= -1;
        }


        // If vertical axis is y
        else if(m_Verticalaxis.y == 1.0)
        {
            m_phi = asin(m_orientation.y);
            m_theta = acos(m_orientation.z / cos(m_phi));

            if(m_orientation.z < 0)
                m_theta *= -1;
        }


        // If vertical axis is z
        else
        {
            m_phi = asin(m_orientation.x);
            m_theta = acos(m_orientation.z / cos(m_phi));

            if(m_orientation.z < 0)
                m_theta *= -1;
        }
        m_phi = m_phi * 180 / M_PI;
        m_theta = m_theta * 180 / M_PI;
    }


    void Camera::setPosition(glm::vec3 position)
    {
        // Position update
        m_position = position;

        // Target
        m_target = m_position + m_orientation;
    }

    void Camera::setSensivity(float sensivity)
    {
        sensivity = sensivity;
    }

}
