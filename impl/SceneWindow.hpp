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

    void keyPressEvent(QKeyEvent * e);

    void setModel(QString);
    void runAnimation(int i);
    void addAnimation(const std::string& file_name);
    scene::SpotLight::Light* getLight();

    AnimMesh * myMesh;
    Triangle * triangle;
     Triangle * triangle2;
    scene::Node * subNode;

    float factor;

};

#endif // SCENEWINDOW_HPP
