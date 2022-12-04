//
// Created by Thuthuka on 2022/12/03.
//

#include "Collideable.h"

Collideable::Collideable(GameObject *gameObject1) : gameObject(gameObject1) {}

Interval Collideable::getXInterval() { //default, should be overidden
    return Interval{gameObject->position.x - 1, gameObject->position.x + 1};
}
