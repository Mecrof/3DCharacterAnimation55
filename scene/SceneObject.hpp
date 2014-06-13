#ifndef SCENEOBJECT_HPP
#define SCENEOBJECT_HPP

// GLM includes
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
// Project includes
#include "scene/Shader.hpp"

namespace scene
{
    class SceneObject
    {
    public:
        explicit SceneObject();
        ~SceneObject();

        virtual void update(float tpf) = 0;
        virtual void render(const glm::mat4 & modelView, const glm::mat4 & projection);

    protected:
        //Node * m_parent;
        float * m_Vertices;
        float * m_Colors;
        Shader * m_Shader;
    };
}

#endif // SCENEOBJECT_HPP
