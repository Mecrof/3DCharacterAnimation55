#include "SceneWindow.hpp"

SceneWindow::SceneWindow(QWidget *parent)
    : GLRenderer(parent, 800, 600, "Opengl Engine v0.1")
{
    factor = 1.0f;
    time = 0.0f;
}

void SceneWindow::initialize()
{
    triangle = new Triangle();
    triangle2 = new Triangle();
    this->getRootNode().setObject(triangle);
    this->getRootNode().translate(glm::vec3(0.0f,0.0f,-1.0f));
    subNode = new scene::Node();
    subNode->setParent(this->getRootNode());
    subNode->setName("subNode");
    this->getRootNode().attachNode((*subNode));

    subNode->setObject(triangle2);
    subNode->translate(glm::vec3(-1.0f,0.0f,0.0f));
    subNode->scale(glm::vec3(-0.5f,-0.5f,0.0f));
}

void SceneWindow::update(float tpf)
{
    scene::Node & rootnode = getRootNode();
    rootnode.rotate(M_PI_2*tpf, glm::vec3(0.0f, 0.0f, 1.0f));

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


