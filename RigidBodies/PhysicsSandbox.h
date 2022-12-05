//
// Created by Thuthuka on 2022/12/05.
//

#ifndef FAKEBASKETBALL_PHYSICSSANDBOX_H
#define FAKEBASKETBALL_PHYSICSSANDBOX_H

#include "RigidBody.h"
#include <vector>

#include "../Collideable/CollisionSandbox.h"

class PhysicsSandbox {
private:
    CollisionSandbox* collisionSandbox;
    std::vector<RigidBody*> rigidBodies;
    float lastTime = 0.0f;
public:
    PhysicsSandbox();
    void addRigidBody(RigidBody* rigidBody);
    void progressPhysics(float newTime);
};


#endif //FAKEBASKETBALL_PHYSICSSANDBOX_H
