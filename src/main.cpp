#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "GLShader.h"

#define WINDOW_HEIGHT 800
#define WINDOW_WIDTH 800

void error_callback(int error, const char* description);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

int main() {
    if (!glfwInit()) {
        std::cerr << "GLFW initialization failed" << std::endl;
        return 1;
    }
    glfwSetErrorCallback(error_callback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Mandelbrot", NULL, NULL);
    if (!window) {
        std::cerr << "GLFW window creation failed" << std::endl;
        return 1;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);

    glfwSetKeyCallback(window, key_callback);

    GLShader* basic2dVertShader = new GLShader(GL_VERTEX_SHADER, "./src/shaders/basic2d.vs");
    GLShader* solidColorFragShader = new GLShader(GL_FRAGMENT_SHADER, "./src/shaders/mandelbrot.fs");

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, basic2dVertShader->getId());
    glAttachShader(shaderProgram, solidColorFragShader->getId());
    glLinkProgram(shaderProgram);
    
    /* assuming no link errors... */
    glDetachShader(shaderProgram, basic2dVertShader->getId());
    glDetachShader(shaderProgram, solidColorFragShader->getId());
    glDeleteShader(basic2dVertShader->getId());
    glDeleteShader(solidColorFragShader->getId());

    float squareVertices[] = {
       1.0f, 1.0f,
       1.0f, -1.0f,
       -1.0f, -1.0f,
       -1.0f, 1.0f, 
    };
    unsigned int squareIndices[] = {
        0, 1, 3,
        1, 2, 3
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices), squareVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(squareIndices), squareIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(window)) {
        int frameBufferWidth, frameBufferHeight;
        glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
        glViewport(0, 0, frameBufferWidth, frameBufferHeight);

        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        GLuint resolutionUniform = glGetUniformLocation(shaderProgram, "resolution");
        glUniform2f(resolutionUniform, frameBufferWidth, frameBufferHeight);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

void error_callback(int error, const char* description) {
    std::cerr << "GLFW error: " << description << std::endl;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}