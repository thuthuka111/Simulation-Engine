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
#include "Collideable.h"

struct collisionID { //TODO change objects to have unique identiferis to save space
    Collideable* object1;
    Collideable* object2;
};

class CollisionSandbox {
private:
    unsigned int collisionCounter = 0;
    std::vector<Collideable*> collisionObjects;
    collisionID recentCollisionHistory[5];
    bool objectsRecentlyCollided(Collideable* object1, Collideable* object2);
public:
    void addCollider(Collideable* object);
    void resolveCollisions();
};


#endif //FAKEBASKETBALL_COLLISIONSANDBOX_H
