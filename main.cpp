
//#include <memory>
#include <iostream>
#include "glm/gtx/rotate_vector.hpp"
#include "OpenGL.h"
#include "InputHandler/Game.h"
#include "server.h"

int main() {
    networking::server_start();
    auto *openGlContext = new OpenGL();
    auto *physicsSandbox = new PhysicsSandbox();

    //Assigning the Sandboxes, which must be the same thing the same thing
    openGlContext->setPhysicsSandbox(physicsSandbox);

    // Game Setup
    auto game = Game(openGlContext, physicsSandbox);
    int returnCode = game.start2BallsGame();
    delete openGlContext;

    return returnCode;
}
