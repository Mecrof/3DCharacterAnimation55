#include "Node.hpp"

#include <iostream>

namespace scene
{
    Node::Node():Spatial()
    {
        m_Parent = NULL;
        m_Children = new QLinkedList<Node*>;
        m_Object = NULL;
    }

    Node::~Node()
    {
        if (!m_Children->isEmpty())
        {
            for(QLinkedList<Node*>::iterator it = m_Children->begin();
                it != m_Children->end();
                ++it)
            {
                delete (*it);
            }
        }
        delete m_Children;
    }

    void Node::render(glm::mat4 modelView, glm::mat4 projection)
    {
        QLinkedList<Node*>::iterator it;
        Node * child;

        modelView = glm::translate(modelView, m_Position);
        modelView *= glm::toMat4(m_Rotation);
        modelView = glm::scale(modelView, m_Scale);
        if (this->hasObject())
        {
            m_Object->render(modelView, projection);
        }
        if (!m_Children->isEmpty())
        {
            for (it = m_Children->begin(); it != m_Children->end(); ++it)
            {
                child = (*it);
                child->render(modelView, projection);
            }
        }
    }

    SceneObject &Node::getObject()
    {
        return (*m_Object);
    }

    SceneObject *Node::setObject(SceneObject *object)
    {
        SceneObject * old = m_Object;
        m_Object = object;
        return old;
    }

    Node *Node::attachNode(Node &n)
    {
        if ( &n != this && !this->m_Children->contains(&n))
        {
            this->m_Children->append(&n);
        }
        return this;
    }

    void Node::setParent(Node &p)
    {
        m_Parent = &p;
    }

    bool Node::hasObject()
    {
        return (m_Object != NULL && m_Object != 0);
    }

    QLinkedList<Node *> *Node::getChildren()
    {
        return m_Children;
    }

    void Node::setName(char *n)
    {
        m_Name = n;
    }

    char *Node::getName()
    {
        return m_Name;
    }

}
