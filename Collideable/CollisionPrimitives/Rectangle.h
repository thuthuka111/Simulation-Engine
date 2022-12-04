//
// Created by Thuthuka on 2022/12/04.
//

#ifndef FAKEBASKETBALL_RECTANGLE_H
#define FAKEBASKETBALL_RECTANGLE_H

#include "../Collideable.h"

class Rectangle: public Collideable {
private:
    unsigned int width;
    unsigned int height;
public:
    Rectangle(GameObject* gameObject, unsigned int width, unsigned int height);
    bool isIntersecting(Rectangle* object2);
};


#endif //FAKEBASKETBALL_RECTANGLE_H
