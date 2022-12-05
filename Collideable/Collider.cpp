//
// Created by Thuthuka on 2022/12/03.
//

#include "Collider.h"
#include "CollideAction.h"
#include "../RigidBodies/RigidBody.h"

Collider::Collider(RigidBody *parentRigidBody) : parentRigidBody(parentRigidBody) {}

Interval Collider::getXInterval() { //default, should be overidden
    return Interval{parentRigidBody->position.x - 1, parentRigidBody->position.x + 1};
}

bool Collider::isIntersecting(Collider *object2) {
    switch (object2->collisionType) {
        case RECTANGLE:
            return isIntersecting((RectangleCollider *) object2);
//        case POINT:
//            return isIntersecting((PointCollider *) object2);
//        case CIRCLE:
//            return isIntersecting((CircleCollider *) object2);
    }
}

void Collider::tryCollision(Collider *otherObject) {
    this->collideAction->reactToCollision(this, otherObject);
}

void Collider::resolveCollision() {
    this->collideAction->applyCollision();
}
