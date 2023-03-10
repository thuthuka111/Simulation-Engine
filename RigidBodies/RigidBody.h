//
// Created by Thuthuka on 2022/11/27.
//

#ifndef FAKEBASKETBALL_RIGIDBODY_H
#define FAKEBASKETBALL_RIGIDBODY_H

#include "glm/glm.hpp"
#include <utility>
#include <vector>

#include "../Collideable/ConcreteColliders/ConcreteColliders.h"

struct hexColour {
    int red;
    int green;
    int blue;
};

class RigidBody {
public:
    //These variables should probably in some other class
    glm::vec2 position = glm::vec2(0.0f, 0.0f);
    glm::vec2 velocity = glm::vec2(0.0f, 0.0f);
    glm::vec2 acceleration = glm::vec2(0.0f, 0.0f);
    float rotation = 0.0f; // Should change this to use quaternions
    float mass = 1.0f;
    Collider* collider;
    bool isStatic = false;
    //hexColour colour = hexColour{255, 255, 255};
    hexColour colour = hexColour{11, 168, 26};

    void setCollisionAction(CollideAction* newCollideAction);
    void addTrigger(std::function<void(Collider*, Collider*)> func);
    virtual std::vector<float> getVertexData() = 0;
};

#endif //FAKEBASKETBALL_RIGIDBODY_H
