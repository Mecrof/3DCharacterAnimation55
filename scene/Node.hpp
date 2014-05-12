#ifndef NODE_H
#define NODE_H

// OpenGL includes
// GLM includes
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
// Qt includes
#include <QLinkedList>
// Projet includes
#include "scene/SceneObject.hpp"

namespace scene {
    class Node
    {
    public:
        Node();
        ~Node();

        void render(glm::mat4 modelView, glm::mat4 projection);

        glm::vec3 *translate(glm::vec3 vector);
        glm::quat *rotate(float angle, glm::vec3 axis);
        glm::vec3 *scale(glm::vec3 factors);

        SceneObject& getObject();
        SceneObject * setObject(SceneObject * object);
        Node * attachNode(Node & n);
        void setParent(Node & p);

        glm::vec3 &getPosition();
        glm::quat &getRotation();
        glm::vec3 &getScale();
        bool hasObject();
        QLinkedList<Node*> * getChildren();

        void setName(char * n);
        char* getName();

    private:
        Node * m_parent;
        QLinkedList<Node*> *m_children;
        SceneObject * m_object;
        glm::vec3 m_position;
        glm::quat m_rotation;
        glm::vec3 m_scale;

        char* m_name;

    };
}

#endif // NODE_H
