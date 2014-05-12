#include "GLRenderer.hpp"
namespace scene
{
    GLRenderer::GLRenderer(QWidget *parent,
                           int width,
                           int height,
                           char *name)
        :QGLWidget(parent)
    {
        this->setWindowTitle(QString::fromUtf8(name));

        this->resize(width, height);

        m_rootNode = new Node();
        m_rootNode->setName("Root Node");
    }

    GLRenderer::~GLRenderer()
    {
        std::cout << "GLRenderer stopping" << std::endl;
        delete m_rootNode;
        std::cout << "GLRenderer stopped" << std::endl;
    }

    void GLRenderer::keyPressEvent(QKeyEvent *e)
    {
        switch(e->key())
        {
            case Qt::Key_Escape:
                close();
                break;
        }
    }

    Node &GLRenderer::getRootNode()
    {
        return (*m_rootNode);
    }

    void GLRenderer::initializeGL()
    {
        //#ifdef WIN32
        GLenum init_GLEW = glewInit();

        if( init_GLEW != GLEW_OK )
        {
            std::cout << "Error in GLEW initialization : " << glewGetErrorString(init_GLEW) << std::endl;
            Q_ASSERT(init_GLEW != GLEW_OK);
        }
        //#endif

        m_modelView = glm::mat4(1.0);
        this->initialize();
    }

    void GLRenderer::resizeGL(int w, int h)
    {
        glViewport(0,0,w,h);
        m_projection = glm::perspective(45.0, w/static_cast<double>(h), 1.0, 100.0);
    }

    void GLRenderer::paintGL()
    {
        glClear(GL_COLOR_BUFFER_BIT);
        m_rootNode->render(m_modelView, m_projection);
    }

    void GLRenderer::closeEvent(QCloseEvent * e)
    {
        std::cout << "closeEvent" << std::endl;
        emit closing();
    }
}
