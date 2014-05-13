#include "Node.hpp"

#include <iostream>

namespace scene
{
    Node::Node():Spatial()
    {
        m_parent = NULL;
        m_children = new QLinkedList<Node*>;
        m_object = NULL;
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

        modelView = glm::translate(modelView, m_position);
        modelView *= glm::toMat4(m_rotation);
        modelView = glm::scale(modelView, m_scale);
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
