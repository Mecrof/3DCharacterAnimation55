#include "GLRenderer.hpp"
namespace scene
{
/////////////////////////////// PUBLIC ///////////////////////////////////////
//============================= LIFECYCLE ====================================

    GLRenderer::GLRenderer(QWidget *parent,
                           int width,
                           int height,
                           char *name)
        :QGLWidget(parent)
    {
        this->setWindowTitle(QString::fromUtf8(name));

        this->resize(width, height);

        m_RootNode = new Node();
        m_RootNode->setName("Root Node");
    }

    GLRenderer::~GLRenderer()
    {
        delete m_RootNode;
        std::cout << "GLRenderer stopped" << std::endl;
    }

//============================= ATTRIBUTE ACCESSORS ==========================

    /**************************************************************************
    * Name: getRootNode()
    * Description: return the rootNode of the scene graph
    * Inputs: void
    * Returns:
    - value: the rootNode
    **************************************************************************/
    Node &GLRenderer::getRootNode()
    {
        return (*m_RootNode);
    }

//============================= OPERATIONS ===================================


    /**************************************************************************
    * Name: keyPressEvent
    * Description: when the user press a key on his keyboard, this function
    * is called.
    * Inputs:
    - QKeyEvent* e: pointer on the key pressed
    * Returns: void
    **************************************************************************/
    void GLRenderer::keyPressEvent(QKeyEvent *e)
    {
        switch(e->key())
        {
            case Qt::Key_Escape:
                close();
                break;
        }
        if(Qt::Key_Up || Qt::Key_Down || Qt::Key_Left || Qt::Key_Right )
                {
                    m_Cam->move(e);
                }
    }

    /**************************************************************************
    * Name:
    * Description: when the user moves his mouse, this function is called.
    * Inputs:
    - QMouseEvent *event: pointer on the mouse
    * Returns: void
    **************************************************************************/
    void GLRenderer::mouseMoveEvent(QMouseEvent *event)
        {
            if(event->type() == QEvent::MouseMove)
            {
                QMouseEvent *Mouse = static_cast<QMouseEvent*>(event);
                int xRel;
                int yRel;
                xRel = Mouse->x() - m_Mousex;
                yRel = Mouse->y() - m_Mousey;
                int xRelAbs = sqrt(xRel*xRel);
                int yRelAbs = sqrt(yRel*yRel);
                if(xRelAbs > 10 || yRelAbs > 10)
                {
                    xRel = 0;
                    yRel = 0;
                }
                m_Cam->orientation(xRel,yRel,0.5);
                //trackCam->OnMouseMotion(xRel,yRel);
                m_Mousex = Mouse->x();
                m_Mousey = Mouse->y();
            }
        }

    /**************************************************************************
    * Name: closeEvent
    * Description: this slot is called when the window with the OpenGL Canvas
    * is shutting down.
    * Inputs:
    - QCloseEvent * e: pointer on the close event
    * Returns: void
    **************************************************************************/
    void GLRenderer::closeEvent(QCloseEvent * e)
    {
        std::cout << "closeEvent" << std::endl;
        emit closing();
    }


/////////////////////////////// PRIVATE ///////////////////////////////////

//============================= OPERATIONS ===================================

    /**************************************************************************
    * Name:
    * Description:
    * Inputs:
    - parameter1: description of parameter1
    ...
    - parameterN: description of parameterN
    * Returns:
    - value: description of the returned value
    **************************************************************************/
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
        m_Cam = new Camera(glm::vec3(3.0,2.149,2.617), glm::vec3(0,0,0), glm::vec3(0,1,0));
       // trackCam = new TrackCamera();
        m_ModelView = glm::mat4(1.0);
        this->initialize();
    }

    /**************************************************************************
    * Name: resizeGL
    * Description: is called when the OpenGL canvas is resizing.
    * Inputs:
    - int w: the new width of the canvas
    - int h: the new height of the canvas
    * Returns: void
    **************************************************************************/
    void GLRenderer::resizeGL(int w, int h)
    {
        glViewport(0,0,w,h);
        m_Projection = glm::perspective(70.0, w/static_cast<double>(h), 1.0, 100.0);
    }

    /**************************************************************************
    * Name: paintGL
    * Description: called each time the canvas must be rerender
    * Inputs: void
    * Returns: void
    **************************************************************************/
    void GLRenderer::paintGL()
    {
        m_Cam->lookat(m_ModelView);
        //trackCam->look(m_modelView);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable( GL_DEPTH_TEST );
        m_RootNode->render(m_ModelView, m_Projection);
    }
}
