#include "GLRenderer.hpp"
namespace scene
{
    GLRenderer::GLRenderer(QWidget *parent,
                           int width,
                           int height,
                           QGLFormat *format,
                           char *name)
        :QGLWidget(parent)
    {
        this->setWindowTitle(QString::fromUtf8(name));
        //this->setGeometry(0,0,width,height);
        //this->setFixedSize(width, height);
        //this->resize(width, height);

        if (format == 0)
        {
            m_GLFormat = new QGLFormat;
            m_GLFormat->setVersion(3,1);
            m_GLFormat->setDoubleBuffer(true);
            m_GLFormat->setDepthBufferSize(24);
            m_GLFormat->setSwapInterval(1);
        }
        else
        {
            m_GLFormat = format;
        }
        m_GLContext = new QGLContext(*m_GLFormat);
        this->setContext(m_GLContext);

        m_rootNode = new Node();
    }

    GLRenderer::~GLRenderer()
    {
        std::cout << "GLRenderer stopping" << std::endl;
        delete m_rootNode;
        delete m_GLContext;
        delete m_GLFormat;
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
        //m_projection = glm::perspective(45.0, (double)(width()/height()), 1.0, 100.0 );
        m_modelView = glm::mat4(1.0);
        this->resize(800, 600);
        this->initialize();
    }

    void GLRenderer::resizeGL(int w, int h)
    {
        glViewport(0,0,w,h);
        m_projection = glm::perspective(45.0, w/static_cast<double>(h), 1.0, 100.0);
    }

    void GLRenderer::paintGL()
    {
        m_modelView = glm::mat4(1.0);
        if (m_rootNode->hasObject())
        {
            m_modelView = glm::scale(m_modelView, m_rootNode->getScale());
            m_modelView *= glm::toMat4(m_rootNode->getRotation());
            m_modelView = glm::translate(m_modelView, m_rootNode->getPosition());
            m_rootNode->getObject().render(m_modelView, m_projection);
        }
        //TODO: Render the subNodes
    }
}
