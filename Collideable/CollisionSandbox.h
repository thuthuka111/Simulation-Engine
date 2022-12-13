//
// Created by Thuthuka on 2022/12/03.
//

#ifndef FAKEBASKETBALL_COLLISIONSANDBOX_H
#define FAKEBASKETBALL_COLLISIONSANDBOX_H

#include <vector>
#include <chrono>
#include "Collider.h"

struct CollisionID { //TODO change objects to have unique identiferis to save space
    Collider* object1;
    Collider* object2;
    std::chrono::time_point<std::chrono::steady_clock> collisionTime;
};

class CollisionSandbox {
private:
    unsigned int collisionCounter = 0;
    std::vector<Collider*> collisionObjects;
    CollisionID recentCollisionHistory[5];
    std::chrono::time_point<std::chrono::steady_clock> steadyClock;
    bool objectsRecentlyCollided(Collider* object1, Collider* object2);
    void recordCollision(Collider* object1, Collider* object2);
public:
    CollisionSandbox();
    void addCollider(Collider* object);
    void resolveCollisions();
};


#endif //FAKEBASKETBALL_COLLISIONSANDBOX_H
