//
// Created by Thuthuka on 2022/12/03.
//

#include "CollisionSandbox.h"

void CollisionSandbox::addCollider(Collideable *object) {
    this->collisionObjects.push_back(object);
}

void
CollisionSandbox::resolveCollisions() { //Sort and Sweep Narrow Phase Collision Detection // WILL BREAK IF 2 THINGS HAVE SAME FLOAT VALUE
    std::list<Collideable *> activeList;
    std::map<float, Collideable *> inactiveList;
    for (auto object: collisionObjects) {
        Interval objInt = object->getXInterval();
        inactiveList.insert({objInt.left, object});
        inactiveList.insert({objInt.right, object});
    }

    for (auto it = inactiveList.begin(); it != inactiveList.end(); ++it) {
        activeList.push_back(it->second);
        if (activeList.size() > 1) {
            auto activeListIT = activeList.begin();
            for (int i = 0; i < activeList.size(); i++) {
                if (!objectsRecentlyCollided(*activeListIT, *(++activeListIT))) {
                    //TODO collide object here
                }
            }
        }
    }
}

bool CollisionSandbox::objectsRecentlyCollided(Collideable *object1, Collideable *object2) {
    for (int i = 0; i < 5; i++) {
        if (recentCollisionHistory[i].object1 == object1 && recentCollisionHistory[i].object2 == object2) {
            return true;
        }
    }
    return false;
}
