#include "SceneWindow.hpp"

SceneWindow::SceneWindow(QWidget *parent)
    : GLRenderer(parent, 800, 600, "Opengl Engine v0.1")
{
    m_Factor = 1.0f;
}

void SceneWindow::initialize()
{
    m_MyMesh = new AnimMesh;

    m_MyMesh->loadMesh("./ninja.b3d");
    this->getRootNode().setObject(m_MyMesh);
    this->getRootNode().scale(0.5f,0.5f,0.5f);
}

void SceneWindow::update(float tpf)
{
    scene::Node & rootnode = getRootNode();
    //rootnode.rotate((M_PI/10.0f)*tpf, glm::vec3(0.0f, 1.0f, 0.0f));
    rootnode.getObject().update(tpf);
}

void
SceneWindow::setModel(QString fileName)
{
    m_MyMesh->loadMesh(fileName.toStdString());
}

void SceneWindow::runAnimation(int i)
{
    this->m_MyMesh->runAnimation(i);
}

void SceneWindow::addAnimation(const std::string &file_name)
{
    this->m_MyMesh->addAnimation(file_name);
}

scene::SpotLight::Light* SceneWindow::getLight()
{
    return this->m_MyMesh->getLight();
}
