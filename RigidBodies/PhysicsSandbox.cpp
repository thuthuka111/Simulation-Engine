//
// Created by Thuthuka on 2022/12/05.
//

#include "PhysicsSandbox.h"

void PhysicsSandbox::addRigidBody(RigidBody *rigidBody) {
    this->rigidBodies.push_back(rigidBody);
}

void PhysicsSandbox::progressPhysics(float currentTime) {//TODO move all the objects, gravity and stuff una;
    float deltaTime = currentTime - lastTime;

    for (auto rigidBody: rigidBodies) {
        rigidBody->velocity += rigidBody->acceleration * deltaTime; // im not so sure why im multiplying deltaTime twice
        rigidBody->position += rigidBody->velocity * deltaTime;
    }

    lastTime = currentTime;
}
