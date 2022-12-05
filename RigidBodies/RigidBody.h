//
// Created by Thuthuka on 2022/11/27.
//

#ifndef FAKEBASKETBALL_RIGIDBODY_H
#define FAKEBASKETBALL_RIGIDBODY_H

#include "glm/glm.hpp"
#include <utility>
#include <vector>

class RigidBody {
public:
    //These variables should probably in some other class
    glm::vec2 position = glm::vec2(0.0f, 0.0f), velocity = glm::vec2(0.0f, 0.0f);
    glm::vec2 acceleration = glm::vec2(0.0f, 0.0f), rotation = glm::vec2(0.0f, 0.0f);
    float mass = 1.0f;

    virtual std::vector<float> getVertices() = 0;
};

#endif //FAKEBASKETBALL_RIGIDBODY_H
