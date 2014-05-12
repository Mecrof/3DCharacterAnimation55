#include "SceneObject.hpp"

namespace scene
{
    /*
    SceneObject::SceneObject(Node *parent)
    {
        if (parent != 0)
        {
            m_parent = parent;
        }
        else
        {
            m_parent = NULL;
        }
    }
    //*/

//*
    SceneObject::SceneObject()
    {
    }
//*/
    SceneObject::~SceneObject()
    {

    }

    void SceneObject::render(glm::mat4 modelView, glm::mat4 projection)
    {

        glUseProgram(m_shader->getProgramID());

            GLint idVertex = glGetAttribLocation(m_shader->getProgramID(), "in_Vertex");
            GLint idColor = glGetAttribLocation(m_shader->getProgramID(), "in_Color");

            glVertexAttribPointer(idVertex, 3, GL_FLOAT, GL_FALSE, 0, m_vertices);
            glEnableVertexAttribArray(idVertex);

            glVertexAttribPointer(idColor, 3, GL_FLOAT, GL_FALSE, 0, m_colors);
            glEnableVertexAttribArray(idColor);

            GLint idModelView = glGetUniformLocation(m_shader->getProgramID(), "modelview");
            glUniformMatrix4fv(idModelView, 1, GL_FALSE, glm::value_ptr(modelView) );
            GLint idProjection = glGetUniformLocation(m_shader->getProgramID(), "projection");
            glUniformMatrix4fv(idProjection, 1, GL_FALSE, glm::value_ptr(projection) );

            glDrawArrays(GL_TRIANGLES, 0, 3);

            glDisableVertexAttribArray(idVertex);
            glDisableVertexAttribArray(idColor);

        glUseProgram(0);
    }
}
