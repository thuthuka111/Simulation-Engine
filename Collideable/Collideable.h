//
// Created by Thuthuka on 2022/12/03.
//

#ifndef FAKEBASKETBALL_COLLIDEABLE_H
#define FAKEBASKETBALL_COLLIDEABLE_H

#include "../RigidBodies//RigidBody.h"
#include "CollideAction.h"

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
private:
    CollideAction* collideAction;
public:
    RigidBody* gameObject;
    CollisionType collisionType;
    Collideable(RigidBody* gameObject1);
    void tryCollision(Collideable*);
    void resolveCollision();
    virtual Interval getXInterval();
    bool isIntersecting(Collideable* object2);
    virtual bool isIntersecting(Rectangle* object2) = 0;
    virtual bool isIntersecting(Circle* object2) = 0;
    virtual bool isIntersecting(Point* object2) = 0;
};


#endif //FAKEBASKETBALL_COLLIDEABLE_H
