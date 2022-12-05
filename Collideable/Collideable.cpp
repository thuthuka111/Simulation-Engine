//
// Created by Thuthuka on 2022/12/03.
//

#include "Collideable.h"

Collideable::Collideable(GameObject *gameObject1) : gameObject(gameObject1) {}

Interval Collideable::getXInterval() { //default, should be overidden
    return Interval{gameObject->position.x - 1, gameObject->position.x + 1};
}

bool Collideable::isIntersecting(Collideable *object2) {
    switch (object2->collisionType) {
        case RECTANGLE:
            return isIntersecting((Rectangle*)object2);
//        case POINT: //TODO add these 'isItersecting's to the class definition
//            return isIntersecting((Point*)object2);
//        case CIRCLE:
//            return isIntersecting((Circle*)object2);
        default:
            return false;
    }
}
