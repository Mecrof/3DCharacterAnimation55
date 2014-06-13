#include "SceneObject.hpp"

namespace scene
{
/////////////////////////////// PUBLIC ///////////////////////////////////////
//============================= LIFECYCLE ====================================
    SceneObject::SceneObject()
    {
    }

    SceneObject::~SceneObject()
    {

    }
//============================= OPERATIONS ===================================
    /**************************************************************************
    * Name: render
    * Description: render the object with the transformation given in the modelView
    * matrix.
    * Inputs:
    - const glm::mat4 &modelView: matrix of the model and the view in the 3D world
    - const glm::mat4 &projection: matrix of the projection in the 3D world
    * Returns: void
    **************************************************************************/
    void SceneObject::render(const glm::mat4 & modelView, const glm::mat4 & projection)
    {

        glUseProgram(m_Shader->getProgramID());

            GLint idVertex = glGetAttribLocation(m_Shader->getProgramID(), "in_Vertex");
            GLint idColor = glGetAttribLocation(m_Shader->getProgramID(), "in_Color");

            glVertexAttribPointer(idVertex, 3, GL_FLOAT, GL_FALSE, 0, m_Vertices);
            glEnableVertexAttribArray(idVertex);

            glVertexAttribPointer(idColor, 3, GL_FLOAT, GL_FALSE, 0, m_Colors);
            glEnableVertexAttribArray(idColor);

            GLint idModelView = glGetUniformLocation(m_Shader->getProgramID(), "modelview");
            glUniformMatrix4fv(idModelView, 1, GL_FALSE, glm::value_ptr(modelView) );
            GLint idProjection = glGetUniformLocation(m_Shader->getProgramID(), "projection");
            glUniformMatrix4fv(idProjection, 1, GL_FALSE, glm::value_ptr(projection) );

            glDrawArrays(GL_TRIANGLES, 0, 3);

            glDisableVertexAttribArray(idVertex);
            glDisableVertexAttribArray(idColor);

        glUseProgram(0);
    }
}
