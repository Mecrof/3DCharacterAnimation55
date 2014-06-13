#include "Camera.hpp"
namespace scene
{
/////////////////////////////// PUBLIC ///////////////////////////////////////
//============================= LIFECYCLE ====================================

    Camera::Camera()
    {
        m_Phi = 0.0f;
        m_Theta = 0.0f;
        m_Orientation = glm::vec3(0.0f, 0.0f, 0.0f);
        m_Verticalaxis = glm::vec3(0.0f, 0.0f, 1.0f);
        m_SideMove = glm::vec3(0.0f, 0.0f, 0.0f);
        m_Position = glm::vec3(0.0f, 0.0f, 0.0f);
        m_Target = glm::vec3(0.0f, 0.0f, 0.0f);
        m_Sensivity = 0.5f;
    }

    Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 verticalaxis)
    {
        m_Phi = -35.26f;
        m_Theta = -135.0f;
        m_Orientation = glm::vec3(0.0f, 0.0f, 0.0f);
        m_Verticalaxis = verticalaxis;
        m_SideMove = glm::vec3(0.0f, 0.0f, 0.0f);
        m_Position = position;
        m_Target = target;
        m_Sensivity = 0.5f;
    }

    Camera::~Camera()
    {

    }
    
//============================= OPERATIONS ===================================
/**************************************************************************
* Name: Orientation
* Description: Move the camera depending of the given inputs
* Inputs:
- parameter1: xRel Relative mouse position on x
- parameter2: yRel Relative mouse position on y
- parameter3: sensibility set the camera movement speed
* Returns: void
**************************************************************************/
    void Camera::orientation(int xRel, int yRel, float sensibility)
    {
        // "-=" for trigonometric angles
        m_Phi   -= yRel*sensibility;
        m_Theta -= xRel*sensibility;

        if(m_Phi > 89.0f)
        {
            m_Phi = 89.0f;
        }
        else if(m_Phi < -89.0f)
        {
            m_Phi = -89.0f;
        }

        float phiRadian = m_Phi * M_PI / 180;
        float thetaRadian = m_Theta * M_PI / 180;

        // If vertical axis is x
        if(m_Verticalaxis.x == 1.0)
        {
            m_Orientation.x = sin(phiRadian);
            m_Orientation.y = cos(phiRadian) * cos(thetaRadian);
            m_Orientation.z = cos(phiRadian) * sin(thetaRadian);
        }


        // If vertical axis is y
        else if(m_Verticalaxis.y == 1.0)
        {
            m_Orientation.x = cos(phiRadian) * sin(thetaRadian);
            m_Orientation.y = sin(phiRadian);
            m_Orientation.z = cos(phiRadian) * cos(thetaRadian);
        }


        // If vertical axis is z
        else
        {
            m_Orientation.x = cos(phiRadian) * cos(thetaRadian);
            m_Orientation.y = cos(phiRadian) * sin(thetaRadian);
            m_Orientation.z = sin(phiRadian);
        }


        // Normal calcul
        m_SideMove = glm::cross(m_Verticalaxis, m_Orientation);
        m_SideMove = glm::normalize(m_SideMove);

        // Target calcul
        m_Target = m_Position + m_Orientation;
    }
    
/**************************************************************************
* Name: Move
* Description: Catch a key event and move the camera
* Inputs:
- parameter1: QEvent key event
* Returns: void
**************************************************************************/
    void Camera::move(QEvent *event)
    {
        if(event->type() == QKeyEvent::KeyPress)
        {
            QKeyEvent *key = static_cast<QKeyEvent *>(event);
            switch(key->key())
            {
                case Qt::Key_Up:
                    m_Position = m_Position + m_Orientation * m_Sensivity;
                    m_Target = m_Position + m_Orientation;
                    break;
                case Qt::Key_Down:
                    m_Position = m_Position - m_Orientation * m_Sensivity;
                    m_Target = m_Position + m_Orientation;
                    break;
                case Qt::Key_Left:
                    m_Position = m_Position + m_SideMove * m_Sensivity;
                    m_Target = m_Position + m_Orientation;
                    break;
                case Qt::Key_Right:
                    m_Position = m_Position - m_SideMove * m_Sensivity;
                    m_Target = m_Position + m_Orientation;
                    break;
            }
       }
    }
    
/**************************************************************************
* Name: LookAt
* Description: Allow the camera to look at the given modelview
* Inputs:
- parameter1: modelview is the view of the world
* Returns: void
**************************************************************************/
    void Camera::lookat(glm::mat4 &modelview)
    {
        modelview = glm::lookAt(m_Position, m_Target, m_Verticalaxis);
    }
    
//============================= ATTRIBUTE ACCESSORS ==========================
    void Camera::setTarget(glm::vec3 target)
    {
        // Orientation vector calcul
        m_Orientation = m_Target - m_Position;
        m_Orientation = glm::normalize(m_Orientation);


        // If vertical axis is x
        if(m_Verticalaxis.x == 1.0)
        {
            m_Phi = asin(m_Orientation.x);
            m_Theta = acos(m_Orientation.y / cos(m_Phi));

            if(m_Orientation.y < 0)
                m_Theta *= -1;
        }


        // If vertical axis is y
        else if(m_Verticalaxis.y == 1.0)
        {
            m_Phi = asin(m_Orientation.y);
            m_Theta = acos(m_Orientation.z / cos(m_Phi));

            if(m_Orientation.z < 0)
                m_Theta *= -1;
        }


        // If vertical axis is z
        else
        {
            m_Phi = asin(m_Orientation.x);
            m_Theta = acos(m_Orientation.z / cos(m_Phi));

            if(m_Orientation.z < 0)
                m_Theta *= -1;
        }
        m_Phi = m_Phi * 180 / M_PI;
        m_Theta = m_Theta * 180 / M_PI;
    }


    void Camera::setPosition(glm::vec3 position)
    {
        m_Position = position;

        m_Target = m_Position + m_Orientation;
    }

    void Camera::setSensivity(float sensivity)
    {
        sensivity = sensivity;
    }

}
