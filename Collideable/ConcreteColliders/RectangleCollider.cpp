//
// Created by Thuthuka on 2022/12/04.
//

#include "RectangleCollider.h"

enum CollisionType;

RectangleCollider::RectangleCollider(RigidBody *gameObject, float width, float height) : Collider(gameObject), width(width), height(height) {
    collisionType = CollisionType::RECTANGLE;
}

bool RectangleCollider::isIntersecting(RectangleCollider *object2) {
    //TODO to rectangle rectangle intersection
    return false;
}
