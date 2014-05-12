#include "Node.hpp"

#include <iostream>

namespace scene
{
    Node::Node()
    {
        m_parent = NULL;
        m_children = new QLinkedList<Node*>;
        m_object = NULL;
        m_position = glm::vec3(0.0f);
        m_rotation = glm::angleAxis(0.0f, glm::vec3(0.0f));
        m_scale = glm::vec3(1.0f);

    }

    Node::~Node()
    {
        if (!m_children->isEmpty())
        {
            for(QLinkedList<Node*>::iterator it = m_children->begin();
                it != m_children->end();
                ++it)
            {
                delete (*it);
            }
        }
        delete m_children;
    }

    void Node::render(glm::mat4 modelView, glm::mat4 projection)
    {
        QLinkedList<Node*>::iterator it;
        Node * child;
        modelView = glm::scale(modelView, m_scale);
        modelView *= glm::toMat4(m_rotation);
        modelView = glm::translate(modelView, m_position);
        if (this->hasObject())
        {
            m_object->render(modelView, projection);
        }
        if (!m_children->isEmpty())
        {
            for (it = m_children->begin(); it != m_children->end(); ++it)
            {
                child = (*it);
                child->render(modelView, projection);
            }
        }
    }

    glm::vec3 *Node::translate(glm::vec3 vector)
    {
        m_position += vector;
        return &m_position;
    }

    glm::quat *Node::rotate(float angle, glm::vec3 axis)
    {
        m_rotation *= glm::angleAxis(angle, axis);
        return &m_rotation;
    }

    glm::vec3 *Node::scale(glm::vec3 factors)
    {
        m_scale += factors;
        return &m_scale;
    }

    SceneObject &Node::getObject()
    {
        return (*m_object);
    }

    SceneObject *Node::setObject(SceneObject *object)
    {
        SceneObject * old = m_object;
        m_object = object;
        return old;
    }

    Node *Node::attachNode(Node &n)
    {
        if ( &n != this && !this->m_children->contains(&n))
        {
            this->m_children->append(&n);
        }
        return this;
    }

    void Node::setParent(Node &p)
    {
        m_parent = &p;
    }

    glm::vec3 &Node::getPosition()
    {
        return m_position;
    }

    glm::quat &Node::getRotation()
    {
        return m_rotation;
    }

    glm::vec3 &Node::getScale()
    {
        return m_scale;
    }

    bool Node::hasObject()
    {
        return (m_object != NULL && m_object != 0);
    }

    QLinkedList<Node *> *Node::getChildren()
    {
        return m_children;
    }

    void Node::setName(char *n)
    {
        m_name = n;
    }

    char *Node::getName()
    {
        return m_name;
    }

}
