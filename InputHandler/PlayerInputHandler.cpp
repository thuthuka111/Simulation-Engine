//
// Created by Thuthuka on 2023/01/18.
//

#include "PlayerInputHandler.h"
#include <GLFW/glfw3.h>

PlayerInputHandler::PlayerInputHandler(RigidBody *rigidBodyBeingControlled) {
    this->rigidBodyBeingControlled = rigidBodyBeingControlled;
}

void PlayerInputHandler::doPlayerAction() {
    const float multiFactor = 0.002f;
    if (this->RIGHT) {
        this->rigidBodyBeingControlled->velocity.x += multiFactor;
    }
    if (this->LEFT) {
        this->rigidBodyBeingControlled->velocity.x -= multiFactor;
    }
    if(this->UP) {
        this->rigidBodyBeingControlled->velocity.y += multiFactor;
    }
    if(this->DOWN) {
        this->rigidBodyBeingControlled->velocity.y -= multiFactor;
    }
}

void PlayerInputHandler::handleInput(int key, int status, int action, int mods) {
    if (action == GLFW_RELEASE || action == GLFW_PRESS)
        switch (key) {
            case GLFW_KEY_W:
                this->UP = !this->UP;
                break;
            case GLFW_KEY_A:
                this->LEFT = !this->LEFT;
                break;
            case GLFW_KEY_S:
                this->DOWN = !this->DOWN;
                break;
            case GLFW_KEY_D:
                this->RIGHT = !this->RIGHT;
                break;
        }
}
