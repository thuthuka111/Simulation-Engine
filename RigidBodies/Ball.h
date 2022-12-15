//
// Created by Thuthuka on 2022/12/05.
//

#ifndef FAKEBASKETBALL_BALL_H
#define FAKEBASKETBALL_BALL_H

#include "RigidBody.h"

class Ball: public RigidBody {
private:
    float radius;
    int vertices = 64; // roundness of the ball
    std::vector<std::pair<float, float>> getUnusableVertices();
    std::vector<unsigned int> getIndices();
public:
    Ball(float radius);
    virtual std::vector<float> getVertexData();
};


#endif //FAKEBASKETBALL_BALL_H
