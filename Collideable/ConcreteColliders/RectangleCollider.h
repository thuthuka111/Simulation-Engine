//
// Created by Thuthuka on 2022/12/04.
//

#ifndef FAKEBASKETBALL_RECTANGLECOLLIDER_H
#define FAKEBASKETBALL_RECTANGLECOLLIDER_H

#include "../Collider.h"

class RectangleCollider: public Collider {
private:
    float width;
    float height;
public:
    RectangleCollider(RigidBody* gameObject, float width, float height);
    bool isIntersecting(RectangleCollider* object2);
};


#endif //FAKEBASKETBALL_RECTANGLECOLLIDER_H
