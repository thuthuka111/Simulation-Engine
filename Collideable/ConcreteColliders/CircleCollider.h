//
// Created by Thuthuka on 2022/12/04.
//

#ifndef FAKEBASKETBALL_CIRCLECOLLIDER_H
#define FAKEBASKETBALL_CIRCLECOLLIDER_H

#include "../Collider.h"

class CircleCollider: public Collider {
private:
    float radius;
public:
    CircleCollider(RigidBody* gameObject, float radius);
    bool isIntersecting(RectangleCollider* object2);
};


#endif //FAKEBASKETBALL_CIRCLECOLLIDER_H
