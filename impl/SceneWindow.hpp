#ifndef SCENEWINDOW_HPP
#define SCENEWINDOW_HPP

#include "scene/GLRenderer.hpp"
#include "scene/Mesh.hpp"
#include "impl/Triangle.hpp"
#include "scene/AnimMesh.h"
#include <math.h>
#include "scene/Light.hpp"

class SceneWindow : public scene::GLRenderer
{
public:
    explicit SceneWindow(QWidget * parent = 0);

    void initialize();
    void update(float tpf);

    void setModel(QString);
    void runAnimation(int i);
    void addAnimation(const std::string& file_name);
    scene::SpotLight::Light* getLight();

    AnimMesh * m_MyMesh;
    Triangle * m_Triangle;
     Triangle * m_Triangle2;
    scene::Node * m_SubNode;

    float m_Factor;

};

#endif // SCENEWINDOW_HPP
