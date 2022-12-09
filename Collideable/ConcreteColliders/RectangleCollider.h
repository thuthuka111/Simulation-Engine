//
// Created by Thuthuka on 2022/12/04.
//

#ifndef FAKEBASKETBALL_RECTANGLECOLLIDER_H
#define FAKEBASKETBALL_RECTANGLECOLLIDER_H

#include "../Collider.h"

class RectangleCollider: public Collider {
private:
    friend CircleCollider;
    float width;
    float height;
public:
    RectangleCollider(RigidBody* gameObject, float width, float height);
    virtual Interval getXInterval();
    bool isIntersecting(RectangleCollider* object2);
    bool isIntersecting(CircleCollider* object2);
    bool isIntersecting(PointCollider* object2);
};


#endif //FAKEBASKETBALL_RECTANGLECOLLIDER_H
