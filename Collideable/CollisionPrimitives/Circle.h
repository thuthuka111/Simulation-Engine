//
// Created by Thuthuka on 2022/12/04.
//

#ifndef FAKEBASKETBALL_CIRCLE_H
#define FAKEBASKETBALL_CIRCLE_H

#include "../Collideable.h"

class Circle: public Collideable {
private:
    float radius;
public:
    Circle(GameObject* gameObject, float radius);
    bool isIntersecting(Rectangle* object2);
};


#endif //FAKEBASKETBALL_CIRCLE_H
