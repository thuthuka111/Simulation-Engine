//
// Created by Thuthuka on 2022/12/04.
//

#ifndef FAKEBASKETBALL_POINT_H
#define FAKEBASKETBALL_POINT_H

#include "../Collideable.h"

class Point: public Collideable {
public:
    Point(RigidBody* gameObject1);
    bool isIntersecting(Rectangle* object2);
};


#endif //FAKEBASKETBALL_POINT_H
