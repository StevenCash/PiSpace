

#include <iostream>
#include <fstream>

#include "ShaderUtil.h"
//****************************************************************
//Load/compile a shader based on filename
//TBD Throw exceptions for:
//  invalid filename
//  compile failure
//  Other failures?
GLuint createShaderFromFile(GLenum shaderType, const std::string& shaderFile)
{
    //Load the shader file
    std::fstream shaderStream;
    shaderStream.open(shaderFile.c_str());
    std::istreambuf_iterator<char> eos;
    std::string shaderString(std::istreambuf_iterator<char>(shaderStream), eos);
    shaderStream.close();

    //string to keep the type of shader in for later logging
    std::string strShaderType;

    //create the shader handle
    GLuint shaderHandle = 0;
    switch(shaderType)
    {
    case GL_VERTEX_SHADER:
        strShaderType = "vertex";
        break;
    case GL_FRAGMENT_SHADER:
        strShaderType = "fragment";
        break;
    default:
        strShaderType = "unsupported";
        //THROW unsupported shader type exception
        throw 1;
        break;
   }
    shaderHandle = glCreateShader(shaderType);
    //assign the source for this shader to the handle
    const char * strFileData = shaderString.c_str();
    glShaderSource(shaderHandle, 1, &strFileData, NULL);

    //copile the shader and check the results
    glCompileShader(shaderHandle);

    GLint status;
    glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &status);

    if (status == GL_FALSE)
    {
        GLint infoLogLength;
        glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &infoLogLength);

        GLchar *strInfoLog = new GLchar[infoLogLength + 1];
        glGetShaderInfoLog(shaderHandle, infoLogLength, NULL, strInfoLog);

        std::cerr 
            << "Compile failure in " 
            << strShaderType 
            << " shader from file "
            << shaderFile
            << ": "
            << strInfoLog
            << std::endl;

        delete[] strInfoLog;

        //THROW something here
    }

    return shaderHandle;
}


//Take a list of shaders, and put them in a shader program together
GLuint createShaderProgram(const ShaderList_T &shaderList)
{
    GLuint program = glCreateProgram();

    ShaderList_T::const_iterator 
        iter = shaderList.begin(),
        endIter = shaderList.end();

    for(; iter != endIter; ++iter)
    {
        glAttachShader(program, *iter);
    }
    glLinkProgram(program);

    GLint status;
    glGetProgramiv (program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
        //Get/print out the opengl error logging
        GLint infoLogLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

        GLchar strInfoLog[infoLogLength + 1];
        glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
        std::cerr 
            << "Linker failure:" 
            << strInfoLog
            << std::endl;

    }

    for(size_t iLoop = 0; iLoop < shaderList.size(); ++iLoop)
    {
        glDetachShader(program, shaderList[iLoop]);
    }
    return program;
}


void checkGlErrors(int lineno)
{
    GLuint errorCode = glGetError();
    if(errorCode)
    {
        std::cerr << "Line: " << lineno << " - GL Error code: " << errorCode << std::endl;
    }
}
