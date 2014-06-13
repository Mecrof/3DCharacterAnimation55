#ifndef MESH_H
#define MESH_H

#include "SceneObject.hpp"
#include "Light.hpp"

namespace scene
{
    class Mesh : public scene::SceneObject
    {
    public:
       explicit  Mesh(const char*);
        ~Mesh();
        void init();
        void update(float);
        void render(glm::mat4 modelView, glm::mat4 projection);

        Shader &getShader();

    private:
        const char* m_SourceFileName;
        bool m_Initialized;
        SpotLight::Light *m_Light;
        unsigned int m_NumFaces;
        float * m_Normals;
    };

}

#endif // MESH_H
