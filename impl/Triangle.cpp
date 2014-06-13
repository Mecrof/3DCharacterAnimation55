#include "Triangle.hpp"

#include <iostream>
Triangle::Triangle()
{
    m_Vertices = new float[9]{-1.0, -1.0, 0.0,   0.0, 1.0, 0.0,   1.0, -1.0, 0.0};
    m_Colors = new float[9]{1.0, 0.0, 0.0,    0.0, 1.0, 0.0,    0.0, 0.0, 1.0};
    m_Shader = new scene::Shader("./Shaders/couleur3D.vert", "./Shaders/couleur3D.frag");
    m_Shader->bind();
}

void Triangle::update(float tpf)
{
}

float *Triangle::getVert()
{
    return m_Vertices;
}
