#include <QApplication>

#include "impl/SceneWindow.hpp"
#include "engine/Engine.hpp"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    SceneWindow w;
    engine::Engine e(60,&w);
    e.start();
    std::cout << "OpenGL Vendor: " << (char*) glGetString(GL_VENDOR) << "\n";
    std::cout << "OpenGL Renderer: " << (char*) glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL Version: " << (char*) glGetString(GL_VERSION) << std::endl;

    //app.setQuitOnLastWindowClosed(true);
    return app.exec();
}
