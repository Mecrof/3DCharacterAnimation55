#include "Mesh.hpp"
#include "model/Loader.hpp"
#include <iostream>

namespace scene
{
    Mesh::Mesh(const char * sourceFileName) : m_SourceFileName(sourceFileName)
    {
        this->m_Light = new SpotLight::Light;

        this->m_Shader = new scene::Shader("./Shaders/light.vert", "./Shaders/light.frag");
        this->m_Shader->bind();

        this->m_Initialized = false;
    }

    Mesh::~Mesh()
    {

    }

    void
    Mesh::init()
    {
        if(this->m_Initialized)
            return;

        Loader loader;
        vector<glm::vec3> vertices, normals;
        vector<unsigned int> facesVertices, facesNormals;
        bool res;
        glm::vec3 tmp;

        res = loader.loadObj(this->m_SourceFileName, vertices, normals, facesVertices, facesNormals);

        if(!res)
            return;

        this->m_NumFaces = facesVertices.size();
        this->m_Vertices = new float[facesVertices.size() * 3];
        this->m_Normals = new float[facesNormals.size() * 3];

        for(int i=0, j=-1; i<facesVertices.size(); ++i)
        {
            tmp = vertices.at(facesVertices.at(i));
            this->m_Vertices[++j] = tmp.x;
            this->m_Vertices[++j] = tmp.y;
            this->m_Vertices[++j] = tmp.z;
        }

        for(int i=0, j=-1; i<facesNormals.size(); ++i)
        {
            tmp = normals.at(facesNormals.at(i));
            this->m_Normals[++j] = tmp.x;
            this->m_Normals[++j] = tmp.y;
            this->m_Normals[++j] = tmp.z;
        }

        this->m_Initialized = true;
    }

    void
    Mesh::update(float tpf)
    {

    }

    void
    Mesh::render(glm::mat4 modelView, glm::mat4 projection)
    {
        glUseProgram(m_Shader->getProgramID());

            this->m_Light->render(m_Shader->getProgramID());

            GLint idVertex = glGetAttribLocation(m_Shader->getProgramID(), "in_Vertex");
            GLint idNormal = glGetAttribLocation(m_Shader->getProgramID(), "in_Normal");

            glEnableVertexAttribArray(idVertex);
            glVertexAttribPointer(idVertex, 3, GL_FLOAT, GL_FALSE, 0, m_Vertices);

            glEnableVertexAttribArray(idNormal);
            glVertexAttribPointer(idNormal, 3, GL_FLOAT, GL_FALSE, 0, m_Normals);

            GLint idModelView = glGetUniformLocation(m_Shader->getProgramID(), "modelview");
            glUniformMatrix4fv(idModelView, 1, GL_FALSE, glm::value_ptr(modelView) );
            GLint idProjection = glGetUniformLocation(m_Shader->getProgramID(), "projection");
            glUniformMatrix4fv(idProjection, 1, GL_FALSE, glm::value_ptr(projection) );

            glDrawArrays(GL_TRIANGLES, 0, this->m_NumFaces);

            glDisableVertexAttribArray(idVertex);
            glDisableVertexAttribArray(idNormal);

            glUseProgram(0);
    }

    Shader &Mesh::getShader()
    {
        return (*this->m_Shader);
    }
}
