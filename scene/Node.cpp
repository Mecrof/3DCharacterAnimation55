#include "Node.hpp"

#include <iostream>

namespace scene
{
    Node::Node(Node *parent,
               QLinkedList<Node *> * children,
               SceneObject *object,
               glm::vec3 position,
               glm::quat rotation,
               glm::vec3 scale)
    {
        if (parent == 0)
        {
            m_parent = NULL;
        }
        else
        {
            m_parent = parent;
        }

        m_children = children;

        if (object == 0)
        {
            m_object = NULL;
        }
        else
        {
            m_object = object;
        }

        m_position = position;
        m_rotation = rotation;
        m_scale = scale;

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

    glm::vec3 *Node::translate(glm::vec3 vector)
    {
        m_position += vector;
        return &m_position;
    }

    glm::quat *Node::rotate(float angle, glm::vec3 axis)
    {
        //if (m_rotation.w == 0 && m_rotation.)
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

}
