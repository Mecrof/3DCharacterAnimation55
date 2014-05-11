#include "Engine.hpp"
#include <iostream>
namespace engine
{
    Engine::Engine(int fps, scene::GLRenderer *renderer)
    {
        if ( fps == 0 )
        {
            this->m_timer = NULL;
        }
        else
        {
            m_timer = new QTimer(this);
            //connect timeout() signal to clockEvent() slot
            connect(m_timer, SIGNAL(timeout()), this, SLOT(clockEvent()));

        }
        m_fps = fps;
        if (renderer == 0)
        {
            m_renderer = NULL;
        }
        else
        {
            m_renderer = renderer;
        }
        m_time = QTime();
    }

    Engine::~Engine()
    {
        std::cout <<"Timer stopping" <<std::endl;
        /*m_timer->stop();
        m_timer->thread()->exit(0);
        */
        //delete m_timer;
        //delete m_renderer;
        std::cout <<"Timer stopped"<<std::endl;
    }

    void Engine::start()
    {
        m_timer->start(1000/m_fps);//set clock rate
        m_time.start();
        m_renderer->show();
    }

    void Engine::clockEvent()
    {
        int tpf = m_time.elapsed();
        m_time.restart();
        std::cout <<"Time per frame : "<<tpf/1000.0f<<std::endl;
        m_renderer->update(tpf/1000.0f);
        m_renderer->updateGL();

        //m_timer->
        //m_renderer->update();
    }
}
