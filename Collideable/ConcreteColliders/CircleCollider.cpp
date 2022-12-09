//
// Created by Thuthuka on 2022/12/04.
//

#include "CircleCollider.h"
#include "../../RigidBodies/RigidBody.h"

CircleCollider::CircleCollider(RigidBody *gameObject, float radius): Collider(gameObject, CollisionType::CIRCLE), radius(radius) {}

Interval CircleCollider::getXInterval() {
    return Interval{this->parentRigidBody->position.x - this->radius, this->parentRigidBody->position.x + this->radius};
}

bool CircleCollider::isIntersecting(RectangleCollider *object2) {
    return Collider::RECT_CIRCLE(object2->parentRigidBody->position, object2->width, object2->height, object2->parentRigidBody->rotation, this->parentRigidBody->position, this->radius);
}

bool CircleCollider::isIntersecting(CircleCollider *object2) {
    return Collider::CIRCLE_POINT(object2->parentRigidBody->position, this->parentRigidBody->position, this->radius);
}

bool CircleCollider::isIntersecting(PointCollider *object2) {
    return Collider::CIRCLE_POINT(object2->parentRigidBody->position, this->parentRigidBody->position, this->radius);
}
