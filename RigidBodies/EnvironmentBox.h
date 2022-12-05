//
// Created by Thuthuka on 2022/11/27.
//
#ifndef FAKEBASKETBALL_ENVIRONMENTBOX_H
#define FAKEBASKETBALL_ENVIRONMENTBOX_H

#include "RigidBody.h"

class EnvironmentBox : public RigidBody {
private:
    float width, height;

    std::vector<std::pair<float, float>> getUnusableVertices();

    std::vector<unsigned int> getIndices();

public:
    EnvironmentBox(float width, float height);

    virtual std::vector<float> getVertices();
};

#endif