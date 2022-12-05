//
// Created by Thuthuka on 2022/12/03.
//

#ifndef FAKEBASKETBALL_COLLISIONSANDBOX_H
#define FAKEBASKETBALL_COLLISIONSANDBOX_H

#include <vector>
#include <list>
#include <map>
#include <ctime>
#include <utility>
#include "Collider.h"

struct collisionID { //TODO change objects to have unique identiferis to save space
    Collider* object1;
    Collider* object2;
};

class CollisionSandbox {
private:
    unsigned int collisionCounter = 0;
    std::vector<Collider*> collisionObjects;
    collisionID recentCollisionHistory[5];
    bool objectsRecentlyCollided(Collider* object1, Collider* object2);
    void recordCollision(Collider* object1, Collider* object2);
public:
    void addCollider(Collider* object);
    void resolveCollisions();
};


#endif //FAKEBASKETBALL_COLLISIONSANDBOX_H
