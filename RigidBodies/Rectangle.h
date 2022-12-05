//
// Created by Thuthuka on 2022/11/27.
//
#ifndef FAKEBASKETBALL_RECTANGLE_H
#define FAKEBASKETBALL_RECTANGLE_H

#include "RigidBody.h"

class Rectangle: public RigidBody {
private:
    float width, height;

    std::vector<std::pair<float, float>> getUnusableVertices();

    std::vector<unsigned int> getIndices();

public:
    Rectangle(float width, float height);

    virtual std::vector<float> getVertices();
};

#endif //FAKEBASKETBALL_RECTANGLE_H