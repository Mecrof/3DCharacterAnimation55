#ifndef TRACKCAMERA_H
#define TRACKCAMERA_H

// GLM includes
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// OpenGL includes
#include <GL/glew.h>
#include <GL/gl.h>

#include "iostream"

namespace scene
{
    class TrackCamera
    {
    public:
        TrackCamera();

        virtual void OnMouseMotion(int xRel, int yRel);
        //virtual void OnMouseButton();
        //virtual void OnKeyboard();

        virtual void look(glm::mat4 &modelview);
        //virtual void setMotionSensivity();
        //virtual void setScrollSensivity();

        virtual ~TrackCamera();
    protected:
        double m_MotionSensivity;
        double m_ScrollSensivity;
        double m_Distance;
        double m_AngleY;
        double m_AngleZ;
        glm::vec3 m_Eye;
        glm::vec3 m_Center;
        glm::vec3 m_Up;

    };
}
#endif // TRACKCAMERA_H
