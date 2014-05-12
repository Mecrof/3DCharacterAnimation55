#include "SceneWindow.hpp"

SceneWindow::SceneWindow(QWidget *parent)
    : GLRenderer(parent, 800, 600, "Opengl Engine v0.1")
{
    factor = 1.0f;
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
    subNode->setPosition(-1.0f,0.0f,0.0f);
    subNode->setScale(0.5f,0.5f,1.0f);

}

void SceneWindow::update(float tpf)
{
    scene::Node & rootnode = getRootNode();
    rootnode.rotate(M_PI_2*tpf, glm::vec3(0.0f, 0.0f, 1.0f));

    if(subNode->getPosition().x > 1.5f || subNode->getPosition().x < -1.5f)
    {
        factor *= -1;
    }
    subNode->translate(1.5f*tpf*factor,0.0f,0.0f);
    subNode->rotate(M_PI*tpf, glm::vec3(0.0f, 0.0f, 1.0f));

    triangle->update(tpf);
}


