//
// Created by Thuthuka on 2022/12/04.
//

#include "Point.h"

Point::Point(GameObject *gameObject1) : Collideable(gameObject1) {
    collisionType = CollisionType::POINT;
}

bool Point::isIntersecting(Rectangle *object2) {
    //TODO to Circle point intersection
    return false;
}