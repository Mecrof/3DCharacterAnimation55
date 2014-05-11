#include "SceneWindow.hpp"

SceneWindow::SceneWindow(QWidget *parent, QGLFormat *format)
    : GLRenderer(parent, 800, 600, format, "Opengl Engine v0.1")
{
    this->getRootNode().translate(glm::vec3(0.0f,0.0f,-1.0f));
    factor = 1.0f;
    time = 0.0f;
}

void SceneWindow::update(float tpf)
{
    scene::Node & rootnode = getRootNode();
    rootnode.rotate(10.0f*tpf, glm::vec3(0.0f, 0.0f, 1.0f));

    if (time > 2.0f)
    {
        time = 0.0f;
        factor *= -1.0f;
    }
    else
    {
        time += tpf;
    }
    rootnode.scale(glm::vec3(0.5f*tpf*factor, 0.5f*tpf*factor, 0.0f));
    triangle->update(tpf);
}

void SceneWindow::initialize()
{
    triangle = new Triangle();
    this->getRootNode().setObject(triangle);
}
