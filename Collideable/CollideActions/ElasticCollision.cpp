//
// Created by Thuthuka on 2022/12/05.
//

#include "ElasticCollision.h"
#include "../Collider.h"
#include "../../RigidBodies/RigidBody.h"

void ElasticCollision::reactToCollision(Collider *thisObject, Collider *otherObject) {
    this->thisObject = thisObject;

    //TODO do this properly
    this->newVelocity = otherObject->parentRigidBody->velocity;
    this->newVelocity.y *= -1;
}

void ElasticCollision::applyCollision() {
    this->thisObject->parentRigidBody->velocity = newVelocity;
}