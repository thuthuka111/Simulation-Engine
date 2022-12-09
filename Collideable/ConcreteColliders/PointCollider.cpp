//
// Created by Thuthuka on 2022/12/04.
//

#include "PointCollider.h"
#include "../../RigidBodies/RigidBody.h"

PointCollider::PointCollider(RigidBody *gameObject1) : Collider(gameObject1, CollisionType::POINT) {}

bool PointCollider::isIntersecting(RectangleCollider *object2) {
    //TODO to Circle point intersection
    return false;
}

Interval PointCollider::getXInterval() {
    return Interval{this->parentRigidBody->position.x, this->parentRigidBody->position.y};
}

bool PointCollider::isIntersecting(CircleCollider *object2) {
    return Collider::CIRCLE_POINT(this->parentRigidBody->position, object2->parentRigidBody->position, object2->radius);
}

bool PointCollider::isIntersecting(PointCollider *object2) {
    return false;
}
