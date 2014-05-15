#include "Mesh.hpp"
#include "model/Loader.hpp"
#include <iostream>

namespace scene
{
    Mesh::Mesh(const char * sourceFileName) : m_sourceFileName(sourceFileName)
    {
        this->m_light = new SpotLight::Light;

        this->m_shader = new scene::Shader("./Shaders/light.vert", "./Shaders/light.frag");
        this->m_shader->bind();

        this->initialized = false;
    }

    Mesh::~Mesh()
    {

    }

    void
    Mesh::init()
    {
        if(this->initialized)
            return;

        Loader loader;
        vector<glm::vec3> vertices, normals;
        vector<unsigned int> facesVertices, facesNormals;
        bool res;
        glm::vec3 tmp;

        res = loader.loadObj(this->m_sourceFileName, vertices, normals, facesVertices, facesNormals);

        if(!res)
            return;

        this->m_numFaces = facesVertices.size();
        this->m_vertices = new float[facesVertices.size() * 3];
        this->m_normals = new float[facesNormals.size() * 3];

        for(int i=0, j=-1; i<facesVertices.size(); ++i)
        {
            tmp = vertices.at(facesVertices.at(i));
            this->m_vertices[++j] = tmp.x;
            this->m_vertices[++j] = tmp.y;
            this->m_vertices[++j] = tmp.z;
        }

        for(int i=0, j=-1; i<facesNormals.size(); ++i)
        {
            tmp = normals.at(facesNormals.at(i));
            this->m_normals[++j] = tmp.x;
            this->m_normals[++j] = tmp.y;
            this->m_normals[++j] = tmp.z;
        }

        this->initialized = true;
    }

    void
    Mesh::update(float tpf)
    {

    }

    void
    Mesh::render(glm::mat4 modelView, glm::mat4 projection)
    {
        glUseProgram(m_shader->getProgramID());

            this->m_light->render(m_shader->getProgramID());

            GLint idVertex = glGetAttribLocation(m_shader->getProgramID(), "in_Vertex");
            GLint idNormal = glGetAttribLocation(m_shader->getProgramID(), "in_Normal");

            glEnableVertexAttribArray(idVertex);
            glVertexAttribPointer(idVertex, 3, GL_FLOAT, GL_FALSE, 0, m_vertices);

            glEnableVertexAttribArray(idNormal);
            glVertexAttribPointer(idNormal, 3, GL_FLOAT, GL_FALSE, 0, m_normals);

            GLint idModelView = glGetUniformLocation(m_shader->getProgramID(), "modelview");
            glUniformMatrix4fv(idModelView, 1, GL_FALSE, glm::value_ptr(modelView) );
            GLint idProjection = glGetUniformLocation(m_shader->getProgramID(), "projection");
            glUniformMatrix4fv(idProjection, 1, GL_FALSE, glm::value_ptr(projection) );

            glDrawArrays(GL_TRIANGLES, 0, this->m_numFaces);

            glDisableVertexAttribArray(idVertex);
            glDisableVertexAttribArray(idNormal);

        glUseProgram(0);
    }
}
