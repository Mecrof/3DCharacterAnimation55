#include <QApplication>

#include "impl/SceneWindow.hpp"
#include "engine/Engine.hpp"
#include "gui/GUIWindow.hpp"

#include <Importer.hpp>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    gui::GUIWindow window;
    SceneWindow *scene = new SceneWindow();
    engine::Engine e(60, scene);
    e.start();

    std::cout << "OpenGL Vendor: " << (char*) glGetString(GL_VENDOR) << "\n";
    std::cout << "OpenGL Renderer: " << (char*) glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL Version: " << (char*) glGetString(GL_VERSION) << std::endl;

    window.setOpenGLWindow(scene);
    window.show();

    return app.exec();
}
