//
// Created by Thuthuka on 2022/12/03.
//

#ifndef FAKEBASKETBALL_COLLIDER_H
#define FAKEBASKETBALL_COLLIDER_H

class CollideAction;
class RigidBody;
class RectangleCollider;
class PointCollider;
class CircleCollider;

enum CollisionType {
    RECTANGLE,
    POINT,
    CIRCLE,
};

struct Interval {
    float left;
    float right;
};

class Collider {
private:
    CollideAction* collideAction;
    RigidBody* parentRigidBody;
public:
    CollisionType collisionType;
    Collider(RigidBody* gameObject1);
    void tryCollision(Collider*);
    void resolveCollision();
    virtual Interval getXInterval();
    bool isIntersecting(Collider* object2);
    virtual bool isIntersecting(RectangleCollider* object2) = 0;
//    virtual bool isIntersecting(CircleCollider* object2) = 0;
//    virtual bool isIntersecting(PointCollider* object2) = 0;
};


#endif //FAKEBASKETBALL_COLLIDER_H
