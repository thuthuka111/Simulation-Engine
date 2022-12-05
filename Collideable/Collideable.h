//
// Created by Thuthuka on 2022/12/03.
//

#ifndef FAKEBASKETBALL_COLLIDEABLE_H
#define FAKEBASKETBALL_COLLIDEABLE_H

#include "../GameObjects/GameObject.h"

class Rectangle;
class Point;
class Circle;

enum CollisionType {
    RECTANGLE,
    POINT,
    CIRCLE,
};

struct Interval {
    float left;
    float right;
};

class Collideable {
public:
    GameObject* gameObject;
    CollisionType collisionType;
    Collideable(GameObject* gameObject1);
    virtual Interval getXInterval();
    bool isIntersecting(Collideable* object2);
    virtual bool isIntersecting(Rectangle* object2) = 0; //TODO the rest of the types
};


#endif //FAKEBASKETBALL_COLLIDEABLE_H
