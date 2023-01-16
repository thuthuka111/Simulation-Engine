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
#include <ft2build.h>
#include FT_FREETYPE_H

GLuint textShader();

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
            std::vector<float> objectVerts = rigidBody->getVertexData();
            for (auto vert: objectVerts) {
                this->verticeData->push_back(vert);
            }
            this->rigidBodyToTriangleNum[rigidBody] = trianglesIndex{numTrianglesBefore, objectVerts.size() / 6};
            numTrianglesBefore += objectVerts.size() / 6;
        }
    }
    glfwInit();
    setWindowHints();

    GLFWwindow *window = glfwCreateWindow(500, 500, "Thuthuka Engine", NULL, NULL);

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

    // OpenGL state
    // ------------
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // compile and setup the shaders
    // ----------------------------
    GLuint textProgram = textShader();
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height));
    glUseProgram(textProgram);// need to use program to change stuff about it
    glUniformMatrix4fv(glGetUniformLocation(textProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    GLuint shaderProgram = linkShaders();
    glUseProgram(shaderProgram);

    // FreeType
    // --------
    FT_Library ft;
    // All functions return a value different than 0 whenever an error occurred
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return -1;
    }

    // find path to font
    std::string font_name = "C:/Windows/Fonts/Arial.ttf";
    if (font_name.empty())
    {
        std::cout << "ERROR::FREETYPE: Failed to load font_name" << std::endl;
        return -1;
    }

    // load font as face
    FT_Face face;
    if (FT_New_Face(ft, font_name.c_str(), 0, &face)) {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return -1;
    }
    else {
        // set size to load glyphs as
        FT_Set_Pixel_Sizes(face, 0, 48);

        // disable byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // load first 128 characters of ASCII set
        for (unsigned char c = 0; c < 128; c++)
        {
            // Load character glyph
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }
            // generate texture
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                    GL_TEXTURE_2D,
                    0,
                    GL_RED,
                    face->glyph->bitmap.width,
                    face->glyph->bitmap.rows,
                    0,
                    GL_RED,
                    GL_UNSIGNED_BYTE,
                    face->glyph->bitmap.buffer
            );
            // set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // now store character for later use
            Character character = {
                    texture,
                    glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                    glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                    static_cast<unsigned int>(face->glyph->advance.x)
            };
            characters.insert(std::pair<char, Character>(c, character));
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    // destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    // Generate VBOs/VAOs

    glGenVertexArrays(buffers, VAOs);
    glGenBuffers(buffers, VBOs);

    // VAO/VBO configuration for rigidbody objects
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]); // bind to the buffer we generated to this target
    glBufferData(GL_ARRAY_BUFFER, this->verticeData->size() * sizeof(float), this->verticeData->data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // VAO/VBO configuration for texture quads
    // -----------------------------------
    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    // clearing effectively
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // render loop
    // -----------
    glfwSetTime(0.0);
    glViewport(0, 0, width, height);
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();

        // glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Defaults to black
        glClear(GL_COLOR_BUFFER_BIT);

        //gets rid of previously rendered pixels
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Step forward the Physics
        physicsSandbox->progressPhysics(glfwGetTime());

        // RigidBody Render Pass
        glUseProgram(shaderProgram);
        glBindVertexArray(VAOs[0]);

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

        // Text render Pass
        // activate corresponding render state
        auto text1color = glm::vec3(0.3, 0.7f, 0.9f);
        auto text2color = glm::vec3(0.5, 0.8f, 0.2f);
        glUseProgram(textProgram);

        glUniform3f(glGetUniformLocation(textProgram, "textColor"), text1color.x, text1color.y, text1color.z);
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(VAOs[1]);
        renderText("(C) LearnOpenGL.com", 340.0f, 370.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));

        glUniform3f(glGetUniformLocation(textProgram, "textColor"), text2color.x, text2color.y, text2color.z);
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(VAOs[1]);
        renderText("This is sample text", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
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

GLuint textShader() {
    GLuint vertexShader = createAndCompileShader("../shaders/text.vert", GL_VERTEX_SHADER);
    GLuint fragmentShader = createAndCompileShader("../shaders/text.frag", GL_FRAGMENT_SHADER);

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
    delete verticeData;
}

OpenGL::OpenGL() {
    this->verticeData = new std::vector<float>();
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

void OpenGL::renderText(std::string text, float x, float y, float scale, glm::vec3 color)
{
    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = this->characters[*c];

        float xpos = x + ch.bearing.x * scale;
        float ypos = y - (ch.size.y - ch.bearing.y) * scale;

        float w = ch.size.x * scale;
        float h = ch.size.y * scale;
        // update VBO for each character
        float vertices[6][4] = {
                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos,     ypos,       0.0f, 1.0f },
                { xpos + w, ypos,       1.0f, 1.0f },

                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos + w, ypos,       1.0f, 1.0f },
                { xpos + w, ypos + h,   1.0f, 0.0f }
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.textureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
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
