//
// Created by Thuthuka on 2022/11/27.
//

#ifndef FAKEBASKETBALL_GAMEOBJECT_H
#define FAKEBASKETBALL_GAMEOBJECT_H

#include "glm/glm.hpp"
#include <utility>
#include <vector>

class GameObject {
public:
    glm::vec2 position = glm::vec2(0.0f, 0.0f), velocity = glm::vec2(0.0f, 0.0f);
    glm::vec2 acceleration = glm::vec2(0.0f, 0.0f), rotation = glm::vec2(0.0f, 0.0f);
    bool canCollide, staticObject;
    std::vector<GameObject *> canCollideWith;

    virtual std::vector<float> getVertices() = 0;
};

#endif //FAKEBASKETBALL_GAMEOBJECT_H
