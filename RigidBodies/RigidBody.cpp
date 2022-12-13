//
// Created by Thuthuka on 2022/12/09.
//

#include "RigidBody.h"
#include "../Collideable/CollideAction.h"

void RigidBody::setCollisionAction(CollideAction *newCollideAction) {
    if(this->collider->collideAction == nullptr)
        delete this->collider->collideAction;
    this->collider->collideAction = newCollideAction;
}