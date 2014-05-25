#include "SceneWindow.hpp"

SceneWindow::SceneWindow(QWidget *parent)
    : GLRenderer(parent, 800, 600, "Opengl Engine v0.1")
{
    factor = 1.0f;
}

void SceneWindow::keyPressEvent(QKeyEvent *e)
{
    GLRenderer::keyPressEvent(e);
    switch(e->key())
    {
        case Qt::Key_B:
            std::cout << "Rebinding Shader : ... ";
            //myMesh->getShader().bind();
            std::cout << "Done" << std::endl;
            break;
    }
}

void SceneWindow::initialize()
{
    //*
    /*myMesh = new scene::Mesh("./Suzanne.obj");
    myMesh->init();
    */
    myMesh = new AnimMesh;
    myMesh->loadMesh("./AnimatedPill.dae");
//        myMesh->loadMesh("./Juliet_Striped_Bikini/Juliet_Striped_Bikini.dae");
    this->getRootNode().setObject(myMesh);
    this->getRootNode().scale(0.5f,0.5f,0.5f);
    //this->getRootNode().rotate(M_PI_2, glm::vec3(1.0f,0.0f,0.0f));
    //    this->getRootNode().scale(0.025f,0.025f,0.025f);
    //this->getRootNode().setPosition(glm::vec3(0.0f,0.0f,-4.0f));
    //*/
    /*
    triangle = new Triangle();
    triangle2 = new Triangle();
    this->getRootNode().setObject(triangle);
    this->getRootNode().setPosition(glm::vec3(0.0f,0.0f,-5.0f));
    subNode = new scene::Node();
    subNode->setParent(this->getRootNode());
    subNode->setName("subNode");
    this->getRootNode().attachNode((*subNode));

    subNode->setObject(triangle2);
    subNode->setPosition(-1.0f,0.0f,0.0f);
    subNode->setScale(0.5f,0.5f,1.0f);
    //*/
}

void SceneWindow::update(float tpf)
{
    scene::Node & rootnode = getRootNode();
    rootnode.rotate((M_PI/10.0f)*tpf, glm::vec3(0.0f, 1.0f, 0.0f));
    /*
    if(subNode->getPosition().x > 1.5f || subNode->getPosition().x < -1.5f)
    {
        factor *= -1;
    }
    subNode->translate(1.0f*tpf*factor,0.0f,0.0f);
    subNode->rotate(M_PI_2*tpf, glm::vec3(0.0f, 0.0f, 1.0f));

    triangle->update(tpf);
    */
}


