//
// Created by Thuthuka on 2023/01/19.
//

#ifndef FAKEBASKETBALL_GAME_H
#define FAKEBASKETBALL_GAME_H

#include <vector>
#include "../RigidBodies/PhysicsSandbox.h"

class OpenGL;

struct TextElement {
    std::string text;
    float xPos;
    float yPos;
    float scale;
    glm::vec3 color;
};

class Game {
    OpenGL* openGlContext;
    PhysicsSandbox* physicsSandbox;
    std::vector<TextElement*> textElements;
    int leftSideScore = 0, rightSideScore = 0;
public:
    Game(OpenGL* openGLContext, PhysicsSandbox* physicsSandbox);
    int start2BallsGame();
    std::vector<TextElement*>* getTextElements();
};


#endif //FAKEBASKETBALL_GAME_H
