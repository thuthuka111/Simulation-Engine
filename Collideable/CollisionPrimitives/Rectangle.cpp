//
// Created by Thuthuka on 2022/12/04.
//

#include "Rectangle.h"

enum CollisionType;

Rectangle::Rectangle(RigidBody *gameObject, unsigned int width, unsigned int height) : Collideable(gameObject),
                                                                                       width(width), height(height) {
    collisionType = CollisionType::RECTANGLE;
}

bool Rectangle::isIntersecting(Rectangle *object2) {
    //TODO to rectangle rectangle intersection
    return false;
}
