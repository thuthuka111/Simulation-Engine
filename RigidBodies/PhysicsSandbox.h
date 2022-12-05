//
// Created by Thuthuka on 2022/12/05.
//

#ifndef FAKEBASKETBALL_PHYSICSSANDBOX_H
#define FAKEBASKETBALL_PHYSICSSANDBOX_H

#include "RigidBody.h"
#include <vector>

class PhysicsSandbox {
private:
    std::vector<RigidBody*> rigidBodies;
    float lastTime = 0.0f;
public:
    void addRigidBody(RigidBody* rigidBody);
    void progressPhysics(float newTime);
};


#endif //FAKEBASKETBALL_PHYSICSSANDBOX_H
