#ifndef SCENEWINDOW_HPP
#define SCENEWINDOW_HPP

#include "scene/GLRenderer.hpp"
#include "impl/Triangle.hpp"

class SceneWindow : public scene::GLRenderer
{
public:
    explicit SceneWindow(QWidget * parent = 0, QGLFormat * format = 0);

    void update(float tpf);
    void initialize();

    Triangle * triangle;
    float factor;
    float time;


};

#endif // SCENEWINDOW_HPP
