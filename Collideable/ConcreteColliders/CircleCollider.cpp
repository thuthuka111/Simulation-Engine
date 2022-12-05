//
// Created by Thuthuka on 2022/12/04.
//

#include "CircleCollider.h"

CircleCollider::CircleCollider(RigidBody *gameObject, float radius): Collider(gameObject), radius(radius) {
    collisionType = CollisionType::CIRCLE;
}

bool CircleCollider::isIntersecting(RectangleCollider *object2) {
    //TODO to Circle rectangle intersection
    return false;
}