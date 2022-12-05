//
// Created by Thuthuka on 2022/12/04.
//

#include "PointCollider.h"

PointCollider::PointCollider(RigidBody *gameObject1) : Collider(gameObject1) {
    collisionType = CollisionType::POINT;
}

bool PointCollider::isIntersecting(RectangleCollider *object2) {
    //TODO to Circle point intersection
    return false;
}