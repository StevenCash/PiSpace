#ifndef SHADERUTIL_H
#define SHADERUTIL_H

#include "GLHeader.h"

#include <vector>
#include <string>

//SHADER Put the shaders into a program *********************************
typedef std::vector<GLuint> ShaderList_T;


//****************************************************************
//Load/compile a shader based on filename
//TBD Throw exceptions for:
//  invalid filename
//  compile failure
//  Other failures?
GLuint createShaderFromFile(GLenum shaderType, const std::string& shaderFile);

//Take a list of shaders, and put them in a shader program together
GLuint createShaderProgram(const ShaderList_T &shaderList);

void checkGlErrors(int lineno);

#define CHECK_GL checkGlErrors(__LINE__);
#endif // SHADERUTIL_H
