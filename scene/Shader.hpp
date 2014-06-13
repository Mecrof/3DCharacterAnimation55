#ifndef DEF_SHADER
#define DEF_SHADER

// C++ includes
#include <iostream>
#include <string>
#include <fstream>
// OpenGL includes
#include <GL/glew.h>

namespace scene
{
    class Shader
    {
    public:

        Shader();
        Shader(Shader const &s);
        Shader(std::string srcVertexShader, std::string srcFragmentShader);
        ~Shader();

        bool bind();
        bool compile(GLuint &shader, GLenum type, std::string const &srcFile);
        GLuint getProgramID() const;


    private:

        GLuint m_VertexID;
        GLuint m_FragmentID;
        GLuint m_ProgramID;

        std::string m_SrcVertexShader;
        std::string m_SrcFragmentShader;
    };
}

#endif
