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
#include "InputHandler/PlayerInputHandler.h"
#include "InputHandler/Game.h"

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
    int width = 800, height = 800;
    //TODO have a separate class that for things that are displayed
    std::map<RigidBody *, trianglesIndex> rigidBodyToTriangleNum;
    std::vector<RigidBody *> rigidBodies;
    std::vector<float> *verticeData;
    PhysicsSandbox *physicsSandbox;
    bool canRunSomthing = false;
    Game *currentGame;
public:
    std::vector<PlayerInputHandler*> inputHandlers; // should put this in game
    ~OpenGL();

    OpenGL();

    int start();

    static GLuint linkShaders();

    void addRigidBody(RigidBody *);

    void renderText(std::string text, float x, float y, float scale);

    void setPhysicsSandbox(PhysicsSandbox *physicsSandbox);

    void addInputHandler(PlayerInputHandler* inputHandler) {
        this->inputHandlers.insert(this->inputHandlers.end(), inputHandler);
    }

    void setGame(Game* game);
};


#endif //FAKEBASKETBALL_OPENGL_H
