#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "GLShader.h"

GLShader::GLShader(GLenum shaderType, const char* filePath) {
    std::ifstream file;
    std::string fileContents;

    file.exceptions (std::ifstream::failbit | std::ifstream::badbit);

    try {
        file.open(filePath);

        std::stringstream fileStream;
        fileStream << file.rdbuf();
        fileStream << "\0";
        file.close();

        fileContents = fileStream.str();
    } catch (std::ifstream::failure& e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    }

    const char *sourceStr = fileContents.c_str();
    this->_shaderId = glCreateShader(shaderType);
    glShaderSource(this->_shaderId, 1, &sourceStr, NULL);
    glCompileShader(this->_shaderId);

    GLint isCompiled = 0;
    glGetShaderiv(this->_shaderId, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(this->_shaderId, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(this->_shaderId, maxLength, &maxLength, &errorLog[0]);
        std::string errorString(errorLog.begin(), errorLog.end());

        std::cerr << "ERROR::SHADER::COMPILIATION: " << errorString << std::endl;
        glDeleteShader(this->_shaderId);
        return;
    }
}

GLShader::~GLShader() {
    this->deleteShader();
}

void GLShader::deleteShader() {
    glDeleteShader(this->_shaderId);
}

GLuint GLShader::getId() {
    return this->_shaderId;
}