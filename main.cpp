#include <QApplication>

#include "impl/SceneWindow.hpp"
#include "engine/Engine.hpp"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    SceneWindow w;
    engine::Engine e(60,&w);
    e.start();
    //app.setQuitOnLastWindowClosed(true);
    return app.exec();
}
