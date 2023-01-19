//
// Created by Thuthuka on 2023/01/18.
//

#ifndef FAKEBASKETBALL_PLAYERINPUTHANDLER_H
#define FAKEBASKETBALL_PLAYERINPUTHANDLER_H

#include "../RigidBodies/RigidBody.h"

class PlayerInputHandler {
    RigidBody *rigidBodyBeingControlled;
    bool UP = false;
    bool DOWN = false;
    bool LEFT = false;
    bool RIGHT = false;
public:
    PlayerInputHandler(RigidBody *rigidBodyBeingControlled);

    void doPlayerAction();

    void handleInput(int key, int status, int action, int mods);
};


#endif //FAKEBASKETBALL_PLAYERINPUTHANDLER_H
