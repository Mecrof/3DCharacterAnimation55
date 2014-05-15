#ifndef GLRENDERER_H
#define GLRENDERER_H

// OpenGL includes
#include <GL/glew.h>
#include <GL/gl.h>
// GLM includes

// QT includes
#include <QGLWidget>
#include <QKeyEvent>
// Project includes
#include "scene/Node.hpp"
#include "scene/Camera.hpp"

namespace scene
{
    class GLRenderer : public QGLWidget
    {
        Q_OBJECT
    public:
        explicit GLRenderer(QWidget * parent = 0,
                            int width = 800,
                            int height = 600,
                            char * name = 0);
        ~GLRenderer();

        virtual void update(float tpf) = 0;
        virtual void initialize() = 0;
        virtual void keyPressEvent(QKeyEvent * e);

        void mouseMoveEvent(QMouseEvent * event);
        Node& getRootNode();

    signals:
        void closing();

    public slots:
        virtual void closeEvent(QCloseEvent * e);

    private:
        virtual void initializeGL();
        virtual void resizeGL(int w, int h);
        virtual void paintGL();

        int Mousex = 0;
        int Mousey = 0;

        glm::mat4 m_projection;
        glm::mat4 m_modelView;

        Node * m_rootNode;
        Camera * cam;

    };
}
#endif // GLRENDERER_H
