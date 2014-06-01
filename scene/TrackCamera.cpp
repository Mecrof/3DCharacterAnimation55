#include "TrackCamera.hpp"
namespace scene
{


TrackCamera::TrackCamera()
{
    angleY = 0;
    angleZ = 0;
    distance = 5;
    motionSensivity = 0.01;
    scrollSensivity = 1;
    //eye = glm::vec3(distance,0,0);
    eye = glm::vec3(5,0,0);
    center = glm::vec3(0,0,0);
    up = glm::vec3(0,1,0);
}

void
TrackCamera::OnMouseMotion(int xRel, int yRel)
{
    //std::cout << "test" << std::endl;
    angleZ = xRel*motionSensivity;
    angleY = yRel*motionSensivity;
    //float phiangleZ = angleZ * 180 / M_PI;
    //angleY = angleY * 180/ M_PI;
    std::cout << angleY << std::endl;

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
    mx[1][1] = cos(angleY);
    mx[1][2] = -sin(angleY);
    mx[2][1] = sin(angleY);
    mx[2][2] = cos(angleY);
    mx[3][3] = 1;
    glm::mat4 my;
    my[0][0] = cos(angleZ);
    my[1][1] = 1;
    my[0][2] = sin(angleZ);
    my[2][0] = -sin(angleZ);
    my[2][2] = cos(angleZ);
    my[3][3] = 1;
    glm::mat4 mz;
    mz[0][0] = cos(angleY);
    mz[0][1] = -sin(angleY);
    mz[1][0] = sin(angleY);
    mz[1][1] = cos(angleY);
    mz[2][2] = 1;
    mz[3][3] = 1;
    glm::mat4 mRot = mx*my*mz;


    //glm::mat4 mRot = mx*mz*my;


    glm::vec4 eyequat = glm::vec4(eye,1);
    //quat = quat*mRot;
    //eyequat = eyequat*mRot;

    //eyequat = eyequat * my;

    eyequat = eyequat * mz;

    eye.x = eyequat.x;
    eye.y = eyequat.y;
    eye.z = eyequat.z;
    /*
    eye.x = quat.x;
    eye.y = quat.y;
    eye.z = quat.z;
    */
    std::cout << "x = " << eye.x << "; y = " << eye.y << "; z = " << eye.z << std::endl;
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

    modelview = glm::lookAt(eye,center,up);

    //glRotated(angleY,0,1,0);
    //glRotated(angleZ,0,0,1);
    //glm::lookAt(eye,center,up);
}

TrackCamera::~TrackCamera()
{

}

}
