#ifndef NODE_H
#define NODE_H

// OpenGL includes

// GLM includes

// Qt includes
#include <QLinkedList>
// Projet includes
#include "scene/Spatial.hpp"
#include "scene/SceneObject.hpp"

namespace scene {
    class Node : public Spatial
    {
    public:
        Node();
        ~Node();

        void render(glm::mat4 modelView, glm::mat4 projection);

        SceneObject& getObject();
        SceneObject * setObject(SceneObject * object);
        Node * attachNode(Node & n);
        void setParent(Node & p);

        bool hasObject();
        QLinkedList<Node*> * getChildren();

        void setName(char * n);
        char* getName();

    private:
        Node * m_parent;
        QLinkedList<Node*> *m_children;
        SceneObject * m_object;

        char* m_name;

    };
}

#endif // NODE_H
