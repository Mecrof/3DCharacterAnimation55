#ifndef SCENEWINDOW_HPP
#define SCENEWINDOW_HPP

#include "scene/GLRenderer.hpp"
#include "scene/Mesh.hpp"
#include "impl/Triangle.hpp"
#include <math.h>

class SceneWindow : public scene::GLRenderer
{
public:
    explicit SceneWindow(QWidget * parent = 0);

    void initialize();
    void update(float tpf);

    void keyPressEvent(QKeyEvent * e);

    scene::Mesh * myMesh;
    Triangle * triangle;
     Triangle * triangle2;
    scene::Node * subNode;

    float factor;


};

#endif // SCENEWINDOW_HPP
