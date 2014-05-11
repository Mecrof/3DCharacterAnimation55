#include "Shader.hpp"

namespace scene
{
    Shader::Shader() : m_vertexID(0), m_fragmentID(0), m_programID(0), m_srcVertexShader(), m_srcFragmentShader()
    {
    }

    Shader::Shader(Shader const &s)
    {
        m_srcVertexShader = s.m_srcVertexShader;
        m_srcFragmentShader = s.m_srcFragmentShader;

        this->bind();
    }

    Shader::Shader(std::string srcVertexShader, std::string srcFragmentShader) : m_vertexID(0), m_fragmentID(0), m_programID(0),
                                                                           m_srcVertexShader(srcVertexShader), m_srcFragmentShader(srcFragmentShader)
    {
    }

    Shader::~Shader()
    {
        glDeleteShader(m_vertexID);
        glDeleteShader(m_fragmentID);
        glDeleteProgram(m_programID);
    }

    bool Shader::bind()
    {
        if(glIsShader(m_vertexID) == GL_TRUE)
            glDeleteShader(m_vertexID);
        if(glIsShader(m_fragmentID) == GL_TRUE)
            glDeleteShader(m_fragmentID);
        if(glIsProgram(m_programID) == GL_TRUE)
            glDeleteProgram(m_programID);

        if(!compile(m_vertexID, GL_VERTEX_SHADER, m_srcVertexShader))
            return false;

        if(!compile(m_fragmentID, GL_FRAGMENT_SHADER, m_srcFragmentShader))
            return false;

        m_programID = glCreateProgram();

        glAttachShader(m_programID, m_vertexID);
        glAttachShader(m_programID, m_fragmentID);
    /*
        glBindAttribLocation(m_programID, 0, "in_Vertex");
        glBindAttribLocation(m_programID, 1, "in_Color");
        glBindAttribLocation(m_programID, 2, "in_TexCoord0");
    */
        glLinkProgram(m_programID);

        GLint linkError(0);
        glGetProgramiv(m_programID, GL_LINK_STATUS, &linkError);

        //if error
        if(linkError != GL_TRUE)
        {
            GLint errorSize(0);
            glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &errorSize);

            char *error = new char[errorSize + 1];

            glGetShaderInfoLog(m_programID, errorSize, &errorSize, error);
            error[errorSize] = '\0';

            std::cout << error << std::endl;

            delete[] error;
            glDeleteProgram(m_programID);

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
        return m_programID;
    }

}
