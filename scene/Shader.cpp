#include "Shader.hpp"

namespace scene
{
    Shader::Shader() : m_VertexID(0), m_FragmentID(0), m_ProgramID(0), m_SrcVertexShader(), m_SrcFragmentShader()
    {
    }

    Shader::Shader(Shader const &s)
    {
        m_SrcVertexShader = s.m_SrcVertexShader;
        m_SrcFragmentShader = s.m_SrcFragmentShader;

        this->bind();
    }

    Shader::Shader(std::string srcVertexShader, std::string srcFragmentShader) : m_VertexID(0), m_FragmentID(0), m_ProgramID(0),
                                                                           m_SrcVertexShader(srcVertexShader), m_SrcFragmentShader(srcFragmentShader)
    {
    }

    Shader::~Shader()
    {
        glDeleteShader(m_VertexID);
        glDeleteShader(m_FragmentID);
        glDeleteProgram(m_ProgramID);
    }

    bool Shader::bind()
    {
        if(glIsShader(m_VertexID) == GL_TRUE)
            glDeleteShader(m_VertexID);
        if(glIsShader(m_FragmentID) == GL_TRUE)
            glDeleteShader(m_FragmentID);
        if(glIsProgram(m_ProgramID) == GL_TRUE)
            glDeleteProgram(m_ProgramID);

        if(!compile(m_VertexID, GL_VERTEX_SHADER, m_SrcVertexShader))
            return false;

        if(!compile(m_FragmentID, GL_FRAGMENT_SHADER, m_SrcFragmentShader))
            return false;

        m_ProgramID = glCreateProgram();

        glAttachShader(m_ProgramID, m_VertexID);
        glAttachShader(m_ProgramID, m_FragmentID);
    /*
        glBindAttribLocation(m_programID, 0, "in_Vertex");
        glBindAttribLocation(m_programID, 1, "in_Color");
        glBindAttribLocation(m_programID, 2, "in_TexCoord0");
    */
        glLinkProgram(m_ProgramID);

        GLint linkError(0);
        glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &linkError);

        //if error
        if(linkError != GL_TRUE)
        {
            GLint errorSize(0);
            glGetProgramiv(m_ProgramID, GL_INFO_LOG_LENGTH, &errorSize);

            char *error = new char[errorSize + 1];

            glGetShaderInfoLog(m_ProgramID, errorSize, &errorSize, error);
            error[errorSize] = '\0';

            std::cout << error << std::endl;

            delete[] error;
            glDeleteProgram(m_ProgramID);

            return false;
        }
        else
        {
            return true;
        }
    }


    bool Shader::compile(GLuint &shader, GLenum type, std::string const &srcFile)
    {
        //create shader
        shader = glCreateShader(type);
        if(shader == 0)
        {
            std::cout << "Error:Shader: Shader type (" << type << ") does not exist !" << std::endl;
            return false;
        }
        std::ifstream file(srcFile.c_str());
        if(!file)
        {
            std::cout << "Error:Shader: the file " << srcFile << " does not exist" << std::endl;
            glDeleteShader(shader);
            return false;
        }

        std::string line;
        std::string srcCode;

        //reading
        while(getline(file, line))
            srcCode += line + '\n';
        file.close();

        const GLchar* srcCodeChar = srcCode.c_str();
        // push the source to the shader
        glShaderSource(shader, 1, &srcCodeChar, 0);
        // compiling
        glCompileShader(shader);

        GLint compilingError(0);
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compilingError);
        // if error
        if(compilingError != GL_TRUE)
        {
            GLint errorSize(0);
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &errorSize);
            char *error = new char[errorSize + 1];
            glGetShaderInfoLog(shader, errorSize, &errorSize, error);
            error[errorSize] = '\0';
            std::cout << error << std::endl;
            delete[] error;
            glDeleteShader(shader);

            return false;
        }
        else
        {
            return true;
        }
    }


    GLuint Shader::getProgramID() const
    {
        return m_ProgramID;
    }

}
