#include "Triangle.hpp"

#include <iostream>
Triangle::Triangle()
{
    m_vertices = new float[9]{-1.0, -1.0, -1.0,   0.0, 1.0, -1.0,   1.0, -1.0, -1.0};
    m_colors = new float[9]{1.0, 0.0, 0.0,    0.0, 1.0, 0.0,    0.0, 0.0, 1.0};
    m_shader = new scene::Shader("./Shaders/couleur3D.vert", "./Shaders/couleur3D.frag");
    m_shader->bind();
}

void Triangle::update(float tpf)
{
    //std::cout<<"ro"<<std::endl;
    //m_parent->rotate(10.0f*tpf, glm::vec3(0.0f,1.0f,0.0f));
    //m_parent->translate(glm::vec3(0.5f*tpf, 0.0f, 0.0f));
    //std::cout<<"roend"<<std::endl;
    /*
    if (m_vertices[1]>5.0f)
    {
        m_vertices[1] = -0.5f;
    }
    m_vertices[1] += 1.0f * tpf;*/
}

float *Triangle::getVert()
{
    return m_vertices;
}
