//
// Created by Thuthuka on 2022/12/04.
//

#ifndef FAKEBASKETBALL_POINTCOLLIDER_H
#define FAKEBASKETBALL_POINTCOLLIDER_H

#include "../Collider.h"

class PointCollider: public Collider {
public:
    PointCollider(RigidBody* gameObject1);
    bool isIntersecting(RectangleCollider* object2);
};


#endif //FAKEBASKETBALL_POINTCOLLIDER_H
