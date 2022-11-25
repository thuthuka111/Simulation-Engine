//
// Created by Thuthuka on 2022/11/25.
//

#include "OpenGL.h"

#include "GL/glew.h"
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

void setWindowHints();
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void handleKeyInput(GLFWwindow *window, int key, int status, int action, int mods);
GLuint createAndLinkProgram();
GLuint createAndCompileShader(const std::string &fileLocation, GLuint shaderType);
std::string readFileToString(const std::string &fileLocation);

int OpenGL::start() {
    glfwInit();
    setWindowHints();

    GLFWwindow *window = glfwCreateWindow(500, 500, "Fake BasketBall", NULL, NULL);

    if (!window) {
        std::cerr << "GLFW window creation failed" << std::endl;
        glfwTerminate();
        return -2;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, handleKeyInput);

    if (glewInit() != GLEW_OK) {
        std::cout << "Glew initialisation failed" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -3;
    }

    if (!glfwInit()) {
        std::cerr << "GLFW init failed" << std::endl;
        return -1;
    }

    //GLuint shaderProgram = createAndLinkProgram();
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();

        //gets rid of previously rendered pixels
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Transforms -1 to 1 coords to the passed in width and height
        glViewport(0, 0, width, height);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void setWindowHints() {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //to use modern OpenGL context creation
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    // std::cout << "New width and height: " << width << ", " << height << std::endl;
}

void handleKeyInput(GLFWwindow *window, int key, int status, int action, int mods) {
    // handle Key inputs, change this to the playerInputHandler
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

GLuint createAndLinkProgram() {
    GLuint vertexShader = createAndCompileShader("../shaders/shader.vert", GL_VERTEX_SHADER);
    GLuint fragmentShader = createAndCompileShader("../shaders/shader.frag", GL_FRAGMENT_SHADER);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program);

    int success;
    glGetShaderiv(program, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "Shader Program Link error: " << infoLog << std::endl;
    }

    return program;
}

GLuint createAndCompileShader(const std::string &fileLocation, GLuint shaderType) {// creates and compiles fragment and vertex shader
    GLuint shader = glCreateShader(shaderType);
    const std::string shaderSource = readFileToString(fileLocation);
    const char *cShader = shaderSource.c_str();

    glShaderSource(shader, 1, &cShader, nullptr);
    glCompileShader(shader);

    //Debugging purposes
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader Compile error: " << infoLog << std::endl;
    }

    return shader;
}

std::string readFileToString(const std::string &fileLocation) {
    std::ifstream file;
    file.open(fileLocation, std::ios::in);
    std::stringstream contents;

    if (!file.fail()) {
        contents << file.rdbuf();
    } else {
        std::cerr << "Failed to read file" << fileLocation << std::endl;
    }

    return contents.str();
}
