//
// Created by Thuthuka on 2022/12/09.
//

#include "ReflectVelocity.h"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/geometric.hpp"
#include "../Collider.h"
#include "../../RigidBodies/RigidBody.h"

void ReflectVelocity::reactToCollision(Collider *thisObject, Collider *otherObject) {
    this->thisObject = thisObject;
    glm::vec3 normalVec = glm::vec3(0.0f, 1.0f, 0.0f);

    normalVec = glm::rotate(normalVec, otherObject->parentRigidBody->rotation, glm::vec3(0.0f, 0.0f, 1.0f));
    normalVec = glm::normalize(normalVec);
    auto temp = glm::reflect(glm::vec3(thisObject->parentRigidBody->velocity, 0.0f), normalVec);

    this->newVelocity = glm::vec2 (temp.x, temp.y);
}

void ReflectVelocity::applyCollision() {
    this->thisObject->parentRigidBody->velocity = this->newVelocity;
}
