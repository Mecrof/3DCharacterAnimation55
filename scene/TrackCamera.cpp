#include "TrackCamera.hpp"
namespace scene
{

/////////////////////////////// PUBLIC ///////////////////////////////////////
//============================= LIFECYCLE ====================================
TrackCamera::TrackCamera()
{
    m_AngleY = 0;
    m_AngleZ = 0;
    m_Distance = 5;
    m_MotionSensivity = 0.01;
    m_ScrollSensivity = 1;
    //eye = glm::vec3(distance,0,0);
    m_Eye = glm::vec3(5,0,0);
    m_Center = glm::vec3(0,0,0);
    m_Up = glm::vec3(0,1,0);
}

TrackCamera::~TrackCamera()
{

}

//============================= OPERATIONS ===================================
/**************************************************************************
* Name: OnMouseMotion
* Description: Catch the position of the mouse to compute a camera movement
* Inputs: 
- parameter1: xRel relative mouse position on x
- parameter2: yRel relative mouse position on y
* Returns: void
**************************************************************************/
void
TrackCamera::OnMouseMotion(int xRel, int yRel)
{
    m_AngleZ = xRel*m_MotionSensivity;
    m_AngleY = yRel*m_MotionSensivity;

    glm::mat4 mx;
    mx[0][0] = 1;
    mx[1][1] = cos(m_AngleY);
    mx[1][2] = -sin(m_AngleY);
    mx[2][1] = sin(m_AngleY);
    mx[2][2] = cos(m_AngleY);
    mx[3][3] = 1;
    glm::mat4 my;
    my[0][0] = cos(m_AngleZ);
    my[1][1] = 1;
    my[0][2] = sin(m_AngleZ);
    my[2][0] = -sin(m_AngleZ);
    my[2][2] = cos(m_AngleZ);
    my[3][3] = 1;
    glm::mat4 mz;
    mz[0][0] = cos(m_AngleY);
    mz[0][1] = -sin(m_AngleY);
    mz[1][0] = sin(m_AngleY);
    mz[1][1] = cos(m_AngleY);
    mz[2][2] = 1;
    mz[3][3] = 1;
    glm::mat4 mRot = mx*my*mz;

    glm::vec4 eyequat = glm::vec4(m_Eye,1);
    
    eyequat = eyequat * mz;

    m_Eye.x = eyequat.x;
    m_Eye.y = eyequat.y;
    m_Eye.z = eyequat.z;

    std::cout << "x = " << m_Eye.x << "; y = " << m_Eye.y << "; z = " << m_Eye.z << std::endl;
}
/**************************************************************************
* Name: look
* Description: change the camera view to look at the modelview
* Inputs:
- parameter1: modelview to change the camera view
* Returns: void
**************************************************************************/
void
TrackCamera::look(glm::mat4 &modelview)
{
    modelview = glm::lookAt(m_Eye,m_Center,m_Up);
}


}
