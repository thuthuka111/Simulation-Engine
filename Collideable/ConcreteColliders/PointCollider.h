//
// Created by Thuthuka on 2022/12/04.
//

#ifndef FAKEBASKETBALL_POINTCOLLIDER_H
#define FAKEBASKETBALL_POINTCOLLIDER_H

#include "../Collider.h"

class PointCollider: public Collider {
public:
    PointCollider(RigidBody* gameObject1);
    virtual Interval getXInterval();
    bool isIntersecting(RectangleCollider* object2);
    bool isIntersecting(CircleCollider* object2);
    bool isIntersecting(PointCollider* object2);
};


#endif //FAKEBASKETBALL_POINTCOLLIDER_H
