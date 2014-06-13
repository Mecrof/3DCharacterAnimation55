#include "Engine.hpp"
#include <iostream>
namespace engine
{
/////////////////////////////// PUBLIC ///////////////////////////////////////
//============================= LIFECYCLE ====================================
    Engine::Engine(int fps, scene::GLRenderer *renderer)
    {
        if ( fps == 0 )
        {
            this->m_Timer = NULL;
        }
        else
        {
            m_Timer = new QTimer(this);
            //connect timeout() signal to clockEvent() slot
            connect(m_Timer, SIGNAL(timeout()), this, SLOT(clockEvent()));

        }
        m_Fps = fps;
        if (renderer == 0)
        {
            m_Renderer = NULL;
        }
        else
        {
            m_Renderer = renderer;
            connect(m_Renderer, SIGNAL(closing()), this, SLOT(exitEvent()));
        }
        m_Time = QTime();
    }

    Engine::~Engine()
    {
        std::cout <<"Timer stopping" <<std::endl;
        //m_timer->stop();
        delete m_Timer;
        //delete m_renderer;
        std::cout <<"Timer stopped"<<std::endl;
    }

    //============================= OPERATIONS ===================================
    /**************************************************************************
    * Name: start
    * Description: start the engine, the canvas renderere and the timer
    * Inputs: void
    * Returns: void
    **************************************************************************/
    void Engine::start()
    {
        m_Timer->start(1000/m_Fps);//set clock rate
        m_Time.start();
        m_Renderer->show();
    }

    /**************************************************************************
    * Name: clockEvent
    * Description: called when a bip of the timer occures
    * Inputs: void
    * Returns: void
    **************************************************************************/
    void Engine::clockEvent()
    {
        int tpf = m_Time.elapsed();
        m_Time.restart();
        //std::cout <<"FPS : "<<1000.0f/tpf<<std::endl;
        //std::cout <<"Time per frame : "<<tpf/1000.0f<<std::endl;
        m_Renderer->update(tpf/1000.0f);
        m_Renderer->updateGL();
    }

    /**************************************************************************
    * Name: exitEvent
    * Description: called when the GLRenderer is shutting down
    * Inputs: void
    * Returns: void
    **************************************************************************/
    void Engine::exitEvent()
    {
        this->disconnect(m_Renderer, SIGNAL(closing()), this, SLOT(clockEvent()));
        this->disconnect(m_Timer, SIGNAL(timeout()), this, SLOT(clockEvent()));
        m_Timer->stop();
        std::cout << "ExitEvent" << std::endl;
    }
}
