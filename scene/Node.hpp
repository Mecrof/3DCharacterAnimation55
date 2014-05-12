#ifndef NODE_H
#define NODE_H

// OpenGL includes
// GLM includes
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
// Qt includes
#include <QLinkedList>
// Projet includes
#include "scene/SceneObject.hpp"

namespace scene {
    class Node
    {
    public:
        explicit Node(Node * parent = 0,
                      QLinkedList<Node*> * children = new QLinkedList<Node*>(),
                      SceneObject * object = 0,
                      glm::vec3 position = glm::vec3(0.0f),
                      glm::quat rotation = glm::angleAxis(0.0f, glm::vec3(0.0f)),
                      glm::vec3 scale = glm::vec3(1.0f));
        ~Node();

        glm::vec3 *translate(glm::vec3 vector);
        glm::quat *rotate(float angle, glm::vec3 axis);
        glm::vec3 *scale(glm::vec3 factors);

        SceneObject& getObject();
        SceneObject * setObject(SceneObject * object);

        glm::vec3 &getPosition();
        glm::quat &getRotation();
        glm::vec3 &getScale();
        bool hasObject();
        QLinkedList<Node*> * getChildren();

    private:
        Node * m_parent;
        QLinkedList<Node*> *m_children;
        SceneObject * m_object;
        glm::vec3 m_position;
        glm::quat m_rotation;
        glm::vec3 m_scale;

    };
}

#endif // NODE_H
