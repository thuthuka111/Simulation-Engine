//
// Created by Thuthuka on 2022/12/03.
//

#include "CollisionSandbox.h"
#include <iostream>

#include <set>
#include <map>
#include <utility>

struct intervalPoint {
    float point;
    Collider *collider;
};

CollisionSandbox::CollisionSandbox() {
    this->steadyClock = std::chrono::steady_clock::now();
}

bool operator< (intervalPoint const &a, intervalPoint const &b) {
    return a.point < b.point;
};

void CollisionSandbox::addCollider(Collider *object) {
    this->collisionObjects.push_back(object);
}

void
CollisionSandbox::resolveCollisions() { //Sort and Sweep Narrow Phase Collision Detection // WILL BREAK IF 2 THINGS HAVE SAME FLOAT VALUE
    std::set<Collider *> activeList;
    std::multimap<float, Collider*> inactiveList;

    for (auto object: collisionObjects) {
        Interval objInt = object->getXInterval();
        inactiveList.insert({objInt.left, object});
        inactiveList.insert({objInt.right, object});
    }

//    std::cout << "Inactive List: "; /// Used to print the set of intervals
//    for (auto it = inactiveList.begin(); it != inactiveList.end(); ++it) {
//        std::cout << it->first << " ";
//    }
//    std:: cout << std::endl;

    for (auto it = inactiveList.begin(); it != inactiveList.end(); ++it) {
        if(activeList.find(it->second) != activeList.end()) { // Is Already in the active list
            activeList.erase(it->second);
        } else {
            activeList.insert(it->second);

            if (activeList.size() > 1) {
                auto activeListIT = activeList.begin();
                for (int i = 0; i < activeList.size() - 1; i++) {
                    Collider *object1 = *activeListIT;
                    Collider *object2 = *(++activeListIT);

                    if (!objectsRecentlyCollided(object1, object2)) {
                        if (object1->isIntersecting(object2)) {
                            object1->tryCollision(object2);
                            object2->tryCollision(object1);
                            // only apply the effects after both objects know what they are going to change to
                            object1->resolveCollision();
                            object2->resolveCollision();

                            // triggers are run explicitly before the objects collide
                            object1->runTrigger(object2);
                            object2->runTrigger(object1);
                            recordCollision(object1, object2);
                        }
                    }
                }
            }
        }
    }
}

bool CollisionSandbox::objectsRecentlyCollided(Collider *object1, Collider *object2) {
    //if the same objects collide within 2 milliseconds can ignore the collision
    for (int i = 0; i < 5; i++) {
        if (recentCollisionHistory[i].object1 == object1 && recentCollisionHistory[i].object2 == object2) {
            auto millisecondsBetweenCollisions = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - recentCollisionHistory[i].collisionTime).count();
            // std::cout << millisecondsBetweenCollisions << std::endl;
            if(millisecondsBetweenCollisions < 300)
                return true;
        }
    }
    return false;
}

void CollisionSandbox::recordCollision(Collider *object1, Collider *object2) {
    // Record the time after collision in which the collision happened
    for(int i = 0; i < 5; i++) {
        if (recentCollisionHistory[i].object1 == object1 && recentCollisionHistory[i].object2 == object2) {
            this->recentCollisionHistory[i] = CollisionID{object1, object2, std::chrono::steady_clock::now()};
            std::cout << "Recorded duplicate collision at: " << std::chrono::duration_cast<std::chrono::milliseconds>(recentCollisionHistory[i].collisionTime - this->steadyClock).count() << "ms" << std::endl;
            return;
        }
    }
    // If the object collision wasnt in the history
    this->recentCollisionHistory[collisionCounter] = CollisionID{object1, object2, std::chrono::steady_clock::now()};
    std::cout << "Recorded collision at: " << std::chrono::duration_cast<std::chrono::milliseconds>(recentCollisionHistory[collisionCounter].collisionTime - this->steadyClock).count() << "ms" << std::endl;
    collisionCounter = ++collisionCounter % 5;
}


