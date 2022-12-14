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

template <typename T1, typename T2>
struct CollisionID { //TODO change objects to have unique identiferis to save space
    T1 object1;
    T2 object2;
    CollisionID(T1 object1, T2 object2) {
        this->object1 = object1;
        this->object2 = object2;
    }
    bool operator==(const CollisionID &other) const {
        return this->object1 == other.object1 && this->object2 == other.object2;
    }
    //std::chrono::time_point<std::chrono::steady_clock> collisionTime;
};

struct hash_fn {
    template<class T1, class T2>
    std::size_t operator()(const CollisionID<T1, T2> &collisionId) const {
        std::size_t h1 = std::hash<T1>()(collisionId.object1);
        std::size_t h2 = std::hash<T2>()(collisionId.object2);

        return h1 ^ h2;
    }
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
    std::unordered_map<CollisionID<Collider*, Collider*>, std::chrono::time_point<std::chrono::steady_clock>, hash_fn> collisionRecord;
    std::chrono::time_point<std::chrono::steady_clock> steadyClock;
    bool objectsRecentlyCollided(Collider* object1, Collider* object2);
    void recordCollision(Collider* object1, Collider* object2);
public:
    CollisionSandbox();
    void addCollider(Collider* object);
    void resolveCollisions();
};


#endif //FAKEBASKETBALL_COLLISIONSANDBOX_H
