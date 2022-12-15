//
// Created by Thuthuka on 2022/12/03.
//

#ifndef FAKEBASKETBALL_COLLISIONSANDBOX_H
#define FAKEBASKETBALL_COLLISIONSANDBOX_H

#include <vector>
#include <chrono>
#include <map>
#include <unordered_map>
#include "Collider.h"

struct timestamped_collision {
    Collider* otherObject;
    std::chrono::time_point<std::chrono::steady_clock> collisionTime;
};

struct float_collider{
    float position;
    Collider* collider;
};

struct multiSetComp {
    bool operator() (const float_collider &lhs, const float_collider &rhs) const {
        return lhs.position < rhs.position;
    }
};

class CollisionSandbox {
private:
    std::vector<Collider*> collisionObjects;
    std::unordered_map<Collider*, timestamped_collision> objectMostRecentCollision;
    std::chrono::time_point<std::chrono::steady_clock> steadyClock;
    bool objectsRecentlyCollided(Collider* object1, Collider* object2);
    void recordCollision(Collider* object1, Collider* object2);
public:
    CollisionSandbox();
    void addCollider(Collider* object);
    void resolveCollisions();
};


#endif //FAKEBASKETBALL_COLLISIONSANDBOX_H
