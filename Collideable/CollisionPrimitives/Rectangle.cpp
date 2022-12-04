//
// Created by Thuthuka on 2022/12/04.
//

#include "Rectangle.h"

Rectangle::Rectangle(GameObject *gameObject, unsigned int width, unsigned int height) : Collideable(gameObject),
                                                                                        width(width), height(height) {}

bool Rectangle::isIntersecting(Rectangle *object2) {
    //TODO to rectangle rectangle intersection
    return false;
}
