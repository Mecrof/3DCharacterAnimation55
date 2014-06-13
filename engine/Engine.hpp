#ifndef ENGINE_HPP
#define ENGINE_HPP

// Qt includes
#include <QTimer>
#include <QTime>
#include <QThread>
// Project includes
#include "scene/GLRenderer.hpp"

namespace engine
{
    class Engine : public QObject
    {
        Q_OBJECT
    public:
        explicit Engine(int fps = 0, scene::GLRenderer *renderer = 0);
        ~Engine();
        void start();

    public slots:
        virtual void clockEvent();
        virtual void exitEvent();

    private:
        QTimer * m_Timer;
        QTime m_Time;
        scene::GLRenderer * m_Renderer;
        int m_Fps;
    };
}

#endif // ENGINE_HPP
