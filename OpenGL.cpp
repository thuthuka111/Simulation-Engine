//
// Created by Thuthuka on 2022/11/25.
//

#include "OpenGL.h"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void setWindowHints();

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void handleKeyInput(GLFWwindow *window, int key, int status, int action, int mods);

GLuint createAndLinkProgram();

GLuint createAndCompileShader(const std::string &fileLocation, GLuint shaderType);

std::string readFileToString(const std::string &fileLocation);

int OpenGL::start() {
    //Make Vertices Before Window
    {
        unsigned int numTrianglesBefore = 0;
        for (RigidBody *rigidBody: this->rigidBodies) {
            std::vector<float> objectVerts = rigidBody->getVertices();
            for (auto vert: objectVerts) {
                this->vertices->push_back(vert);
            }
            this->rigidBodyToTriangleNum[rigidBody] = trianglesIndex{numTrianglesBefore, objectVerts.size() / 3};
            numTrianglesBefore += objectVerts.size() / 3;
        }
    }
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

    // making the vertex buffer, which stores data and bind it to the target
    GLuint VBO, VAO; // Vertex buffer object

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind to the buffer we generated to this target
    glBufferData(GL_ARRAY_BUFFER, this->vertices->size() * sizeof(float), this->vertices->data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    GLuint shaderProgram = linkShaders();
    glUseProgram(shaderProgram); // Todo Delete the program and the shaders after execution

    glfwSetTime(0.0);
    glViewport(0, 0, width, height);
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();

        glClearColor(0.9f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //gets rid of previously rendered pixels
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Bind Vertex Array Object
        glBindVertexArray(VAO);

        // Step forward the Physics
        physicsSandbox->progressPhysics(glfwGetTime());

        // Put the objects in the right place
        for (auto rigidBody: this->rigidBodies) {
            auto transformation = glm::mat4(1.0f);
            transformation = glm::translate(transformation, glm::vec3(rigidBody->position, 1.0f));
            transformation = glm::rotate(transformation, rigidBody->rotation, glm::vec3(0.0, 0.0, 1.0));


            //TODO bind objects/ids to the number of triangles they have
            GLuint transformLocation = glGetUniformLocation(shaderProgram, "transform");
            glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transformation));

            // Draw the rigidBody
            glDrawArrays(GL_TRIANGLES, rigidBodyToTriangleNum[rigidBody].start, rigidBodyToTriangleNum[rigidBody].size);
        }


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

GLuint OpenGL::linkShaders() { //Create and link Program
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

void OpenGL::addRigidBody(RigidBody *rigidBody) {
    this->rigidBodies.push_back(rigidBody);
}

OpenGL::~OpenGL() {
    delete vertices;
}

OpenGL::OpenGL() {
    this->vertices = new std::vector<float>();
    this->physicsSandbox = new PhysicsSandbox();
}

void OpenGL::setPhysicsSandbox(PhysicsSandbox *physicsSandbox) {
    OpenGL::physicsSandbox = physicsSandbox;
}

GLuint createAndCompileShader(const std::string &fileLocation,
                              GLuint shaderType) {// creates and compiles fragment and vertex shader
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
