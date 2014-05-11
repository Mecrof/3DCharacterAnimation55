#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "scene/SceneObject.hpp"
#include "scene/Node.hpp"

class Triangle : public scene::SceneObject
{
public:
    explicit Triangle();

    void update(float tpf);

    float * getVert();
};

#endif // TRIANGLE_HPP
