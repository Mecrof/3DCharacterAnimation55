CONFIG += console release
QT += opengl

INCLUDEPATH = "../glew-mingw/include" "../glm/"
win32 {
    LIBS += -L"..\glew-mingw\lib" -lglew32
}

SOURCES += \
    main.cpp \
    engine/Engine.cpp \
    scene/GLRenderer.cpp \
    scene/Node.cpp \
    scene/SceneObject.cpp \
    scene/Shader.cpp \
    impl/SceneWindow.cpp \
    impl/Triangle.cpp \
    scene/Spatial.cpp \
    scene/camera.cpp

HEADERS += \
    engine/Engine.hpp \
    scene/GLRenderer.hpp \
    scene/Node.hpp \
    scene/SceneObject.hpp \
    scene/Shader.hpp \
    impl/SceneWindow.hpp \
    impl/Triangle.hpp \
    scene/Spatial.hpp \
    scene/camera.hpp

OTHER_FILES += \
    Shaders/texture.vert \
    Shaders/texture.frag \
    Shaders/couleur3D.vert \
    Shaders/couleur3D.frag \
    Shaders/couleur2D.vert \
    Shaders/couleur2D.frag \
    Shaders/basique2D.vert \
    Shaders/basique2D.frag
