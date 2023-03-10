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
CollisionSandbox::resolveCollisions() { //Sort and Sweep Narrow Phase Collision Detection
    std::set<Collider *> activeList;;
    std::multiset<float_collider, multiSetComp> inactiveList;// map the index in the float array of the Collider to the object

    for (auto object: collisionObjects) {
        Interval objInt = object->getXInterval();
        inactiveList.insert({objInt.left, object});
        inactiveList.insert({objInt.right, object});
    }

//    std::cout << "Inactive List: "; /// Used to print the set of intervals
//    for (auto it = inactiveList.begin(); it != inactiveList.end(); ++it) {
//        std::cout << "{" << it->position << ":" << it->collider << "} ";
//    }
//    std:: cout << std::endl;

    for (auto it = inactiveList.begin(); it != inactiveList.end(); ++it) {
        if(activeList.count(it->collider) == 1) {// Is Already in the active list
            activeList.erase(it->collider);
            continue;
        }
        activeList.insert(it->collider);

        if(activeList.size() <= 1)
            continue;

        // Compare all items in active list to see if they collide
        for(auto i = activeList.begin(); i != activeList.end(); i++) {
            for(auto j = std::next(i); j != activeList.end(); j++) {
                Collider* object1 = *i;
                Collider* object2 = *j;

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
        auto activeListIT = activeList.begin();
    }
}

bool CollisionSandbox::objectsRecentlyCollided(Collider *object1, Collider *object2) {
    if(this->objectMostRecentCollision.count(object1) == 0 || this->objectMostRecentCollision.count(object2) == 0)
        return false; // Neither object has collided with anything
    if(this->objectMostRecentCollision[object1].otherObject == object2 && this->objectMostRecentCollision[object2].otherObject == object1) {
        auto milliSecondsBetweenCollision = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - this->objectMostRecentCollision[object1].collisionTime).count();
        if(milliSecondsBetweenCollision < 100)
            return true;
    }
    // One of the other objects has collided with something else, which means this collision is valid
    return false;
}

void CollisionSandbox::recordCollision(Collider *object1, Collider *object2) {
    // Record the last object an object collided with
    auto nowTime = std::chrono::steady_clock::now();
    this->objectMostRecentCollision[object1] = timestamped_collision{object2, nowTime};
    this->objectMostRecentCollision[object2] = timestamped_collision{object1, nowTime};
}



