//
// Created by Thuthuka on 2022/12/05.
//

#include "PhysicsSandbox.h"

PhysicsSandbox::PhysicsSandbox() : collisionSandbox(new CollisionSandbox()) {}

// maybe somehwere add a specifier whether an objects interracts with other stuff (doesmt make sense)
void PhysicsSandbox::addRigidBody(RigidBody *rigidBody) {
    this->rigidBodies.push_back(rigidBody);
    this->collisionSandbox->addCollider(rigidBody->collider);
}

void PhysicsSandbox::progressPhysics(float currentTime) {// moves all the objects, gravity and stuff una;
    float deltaTime = currentTime - lastTime;

    for (auto rigidBody: rigidBodies) {
        rigidBody->velocity += rigidBody->acceleration * deltaTime; // im not so sure why im multiplying deltaTime twice
        rigidBody->position += rigidBody->velocity * deltaTime;
    }

    //TODO all the intersection Stuff
    collisionSandbox->resolveCollisions();

    lastTime = currentTime;
}
