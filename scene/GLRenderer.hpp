#ifndef GLRENDERER_H
#define GLRENDERER_H

// OpenGL includes
#include <GL/glew.h>
#include <GL/gl.h>
// GLM includes
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
// QT includes
#include <QGLWidget>
#include <QGLFormat>
#include <QGLContext>
#include <QKeyEvent>
// Project includes
#include "scene/Node.hpp"

namespace scene
{
    class GLRenderer : public QGLWidget
    {
        Q_OBJECT
    public:
        explicit GLRenderer(QWidget * parent = 0,
                            int width = 800,
                            int height = 600,
                            QGLFormat * format = 0,
                            char * name = 0);
        ~GLRenderer();

        virtual void update(float tpf) = 0;
        virtual void initialize() = 0;
        virtual void keyPressEvent(QKeyEvent * e);

        Node& getRootNode();

    private:
        virtual void initializeGL();
        virtual void resizeGL(int w, int h);
        virtual void paintGL();

        QGLFormat * m_GLFormat;
        QGLContext * m_GLContext;

        glm::mat4 m_projection;
        glm::mat4 m_modelView;

        Node * m_rootNode;

    };
}
#endif // GLRENDERER_H
