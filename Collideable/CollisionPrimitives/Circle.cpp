//
// Created by Thuthuka on 2022/12/04.
//

#include "Circle.h"

Circle::Circle(GameObject *gameObject, float radius): Collideable(gameObject), radius(radius) {
    collisionType = CollisionType::CIRCLE;
}

bool Circle::isIntersecting(Rectangle *object2) {
    //TODO to Circle rectangle intersection
    return false;
}