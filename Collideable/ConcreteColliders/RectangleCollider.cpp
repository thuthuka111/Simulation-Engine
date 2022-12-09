//
// Created by Thuthuka on 2022/12/04.
//

#include "RectangleCollider.h"
#include "../../RigidBodies/RigidBody.h"

enum CollisionType;

RectangleCollider::RectangleCollider(RigidBody *gameObject, float width, float height) : Collider(gameObject, CollisionType::RECTANGLE), width(width), height(height) {}

Interval RectangleCollider::getXInterval() {
    return Interval{this->parentRigidBody->position.x - this->width / 2, this->parentRigidBody->position.x + this->width / 2}; //TODO make this take the rotation into account
}

bool RectangleCollider::isIntersecting(RectangleCollider *object2) {
    return Collider::RECT_RECT(Rect{this->parentRigidBody->position, this->width, this->height}, this->parentRigidBody->rotation, Rect{object2->parentRigidBody->position, object2->width, object2->height}, object2->parentRigidBody->rotation);
}

bool RectangleCollider::isIntersecting(CircleCollider *object2) {
    return Collider::RECT_CIRCLE(this->parentRigidBody->position, this->width, this->height, this->parentRigidBody->rotation, object2->parentRigidBody->position, object2->radius);
}

bool RectangleCollider::isIntersecting(PointCollider *object2) {
    auto halfWidth = this->width / 2, halfHeight = this->height / 2;
    auto parentPosition = this->parentRigidBody->position;
    glm::vec2 recPoints[3] = {
              glm::vec2 (parentPosition.x - halfWidth, parentPosition.y - halfHeight),//A
              glm::vec2 (parentPosition.x + halfWidth, parentPosition.y - halfHeight),//B
              glm::vec2 (parentPosition.x - halfWidth, parentPosition.y + halfHeight),//C
    };
    return Collider::RECT_POINT(recPoints[0], recPoints[1], recPoints[2], object2->parentRigidBody->position);
}
