#include "TrackCamera.hpp"
namespace scene
{


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

void
TrackCamera::OnMouseMotion(int xRel, int yRel)
{
    //std::cout << "test" << std::endl;
    m_AngleZ = xRel*m_MotionSensivity;
    m_AngleY = yRel*m_MotionSensivity;
    //float phiangleZ = angleZ * 180 / M_PI;
    //angleY = angleY * 180/ M_PI;
    std::cout << m_AngleY << std::endl;

    /*
    float sin_a = sin(xRel / 2);
    float cos_a = cos(xRel / 2);
    glm::vec4 quat;
    quat.x = eye.x * sin_a;
    quat.y = eye.y * sin_a;
    quat.z = eye.z * sin_a;
    quat.w = cos_a;
    glm::normalize(quat);
    */

    /*
    if(angleY > 90)
    {
        angleY = 90;
    }
    else if(angleZ > 90)
    {
        angleZ = 90;
    }
    */
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


    //glm::mat4 mRot = mx*mz*my;


    glm::vec4 eyequat = glm::vec4(m_Eye,1);
    //quat = quat*mRot;
    //eyequat = eyequat*mRot;

    //eyequat = eyequat * my;

    eyequat = eyequat * mz;

    m_Eye.x = eyequat.x;
    m_Eye.y = eyequat.y;
    m_Eye.z = eyequat.z;
    /*
    eye.x = quat.x;
    eye.y = quat.y;
    eye.z = quat.z;
    */
    std::cout << "x = " << m_Eye.x << "; y = " << m_Eye.y << "; z = " << m_Eye.z << std::endl;
}

void
TrackCamera::look(glm::mat4 &modelview)
{
    /*
    glm::vec3 eye = glm::vec3(distance,0,0);
    glm::vec3 center = glm::vec3(0,0,0);
    glm::vec3 up = glm::vec3(0,0,1);
    glm::vec3 F = glm::vec3(center.x - eye.x, center.y - eye.y, center.z - eye.z);
    float fNorm = sqrt(F.x * F.x + F.y * F.y + F.z * F.z);
    glm::vec3 f = glm::vec3(F.x/fNorm,F.y/fNorm,F.z/fNorm);
    float upNorm = sqrt(up.x * up.x + up.y * up.y + up.z * up.z);
    glm::vec3 UP = glm::vec3(up.x/upNorm,up.y/upNorm,up.z/upNorm);
    glm::vec3 s = f * UP;
    float sNorm = sqrt(s.x * s.x + s.y * s.y + s.z * s.z);
    glm::vec3 S = glm::vec3(s.x/sNorm,s.y/sNorm,s.z/sNorm);
    glm::vec3 u = S * f;
    GLfloat M[4][4];
    //glm::mat4 M;
    M[0][0] = s.x; M[0][1] = s.y; M[0][2] = s.z;
    M[1][0] = u.x; M[1][1] = u.y; M[1][2] = u.z;
    M[2][0] = -f.x; M[2][1] = -f.y; M[2][2] = -f.z;
    M[3][3] = 1;
    glMultMatrixf(M[0]);
    glTranslated(-eye.x,-eye.y,-eye.z);
    //modelview = glm::lookAt(eye,center,up);
    //glm::lookAt(eye,center,up);
    glRotated(angleY,0,1,0);
    glRotated(angleZ,0,0,1);
    */
    //glm::vec3 eye = glm::vec3(distance,0,0);
    //glm::vec3 center = glm::vec3(0,0,0);
    //glm::vec3 up = glm::vec3(0,1,0);
    //glOrtho(0,0,0,0,distance,0);

    //glRotatef(angleY,0,1,0);
    //glRotatef(angleZ,0,0,1);
    //glTranslatef(-eye.x,-eye.y,-eye.z);

    modelview = glm::lookAt(m_Eye,m_Center,m_Up);

    //glRotated(angleY,0,1,0);
    //glRotated(angleZ,0,0,1);
    //glm::lookAt(eye,center,up);
}

TrackCamera::~TrackCamera()
{

}

}
