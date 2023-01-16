//
// Created by Thuthuka on 2022/11/25.
//

#ifndef FAKEBASKETBALL_OPENGL_H
#define FAKEBASKETBALL_OPENGL_H

#include <map>

#include "GL/glew.h"
#include <GLFW/glfw3.h>
#include "RigidBodies//RigidBody.h"
#include "Collideable/CollisionSandbox.h"
#include "RigidBodies/PhysicsSandbox.h"

// To hold a FreeFont character
struct Character {
    unsigned int textureID; // ID handle of the glyph texture
    glm::ivec2 size;        // Size of glyph
    glm::ivec2 bearing;     // Offset from baseline to left/top of glyph
    unsigned int advance;   // Offset to advance to next glyph
};

class OpenGL {
    struct trianglesIndex {
        unsigned int start;
        unsigned int size;
    };
    static const unsigned int buffers = 2;
    GLuint VAOs[buffers];
    GLuint VBOs[buffers];
    std::map<char, Character> characters;
    int width = 500, height = 500;
    //TODO have a separate class that for things that are displayed
    std::map<RigidBody *, trianglesIndex> rigidBodyToTriangleNum;
    std::vector<RigidBody *> rigidBodies;
    std::vector<float> *verticeData;
    PhysicsSandbox *physicsSandbox;
public:
    ~OpenGL();

    OpenGL();

    int start();

    static GLuint linkShaders();

    void addRigidBody(RigidBody *);

    void renderText(std::string text, float x, float y, float scale, glm::vec3 color);

    void setPhysicsSandbox(PhysicsSandbox *physicsSandbox);
};


#endif //FAKEBASKETBALL_OPENGL_H
