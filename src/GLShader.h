#ifndef GLSHADER_H
#define GLSHADER_H

#include <glad/gl.h>

class GLShader {
    private:
        GLuint _shaderId;
    public:
        GLShader(GLenum shaderType, const char* filePath);
        ~GLShader();
        void deleteShader();
        GLuint getId();
};

#endif
