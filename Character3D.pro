CONFIG += console release
QT += opengl

INCLUDEPATH = "../glew-mingw/include" "../glm/" "..\assimp\include"
win32 {
    LIBS += -L"..\glew-mingw\lib" -lglew32 -L"..\assimp\lib" -llibassimp
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
    model/Loader.cpp \
    scene/Mesh.cpp \
    scene/Light.cpp \
    scene/Camera.cpp \
    scene/AnimMesh.cpp \
    scene/TrackCamera.cpp \
    scene/Texture.cpp \
    gui/GUIWindow.cpp

HEADERS += \
    engine/Engine.hpp \
    scene/GLRenderer.hpp \
    scene/Node.hpp \
    scene/SceneObject.hpp \
    scene/Shader.hpp \
    impl/SceneWindow.hpp \
    impl/Triangle.hpp \
    scene/Spatial.hpp \
    model/Loader.hpp \
    scene/Mesh.hpp \
    scene/Light.hpp \
    scene/Camera.hpp \
    scene/AnimMesh.h \
    scene/TrackCamera.hpp \
    scene/Texture.hpp\
    gui/GUIWindow.hpp

OTHER_FILES += \
    Shaders/texture.vert \
    Shaders/texture.frag \
    Shaders/couleur3D.vert \
    Shaders/couleur3D.frag \
    Shaders/couleur2D.vert \
    Shaders/couleur2D.frag \
    Shaders/basique2D.vert \
    Shaders/basique2D.frag \
    Shaders/light.vert \
    Shaders/light.frag \
    Suzanne.obj \
    AnimatedPill.dae \
    AnimatedPantin4.dae
