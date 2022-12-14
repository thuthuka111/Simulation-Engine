//
// Created by Thuthuka on 2022/12/05.
//

#include "ElasticCollision.h"
#include "../Collider.h"
#include "../../RigidBodies/RigidBody.h"
#include "glm/gtx/norm.hpp"
#include "glm/gtx/rotate_vector.hpp"


void ElasticCollision::reactToCollision(Collider *thisObject, Collider *otherObject) {
    //https://en.wikipedia.org/wiki/Elastic_collision#Two-dimensional_collision_with_two_moving_objects
    this->thisObject = thisObject;

    if(otherObject->parentRigidBody->isStatic) {
        //TODO the physics part of these Colliide Actions with Static functions that return results
        // Replace this with the Reflect Velocity function
        this->thisObject = thisObject;
        glm::vec3 normalVec = glm::vec3(0.0f, 1.0f, 0.0f);

        normalVec = glm::rotate(normalVec, otherObject->parentRigidBody->rotation, glm::vec3(0.0f, 0.0f, 1.0f));
        normalVec = glm::normalize(normalVec);
        auto temp = glm::reflect(glm::vec3(thisObject->parentRigidBody->velocity, 0.0f), normalVec);

        this->newVelocity = glm::vec2 (temp.x, temp.y);

        return;
    }

    //TODO could make these pointers to save space
    Point x1 = thisObject->parentRigidBody->position, x2 = otherObject->parentRigidBody->position;
    glm::vec2 v1 = thisObject->parentRigidBody->velocity, v2 = otherObject->parentRigidBody->velocity;
    float m1 = thisObject->parentRigidBody->mass, m2 = otherObject->parentRigidBody->mass;

    this->newVelocity = v1 - ((2 * m2) / (m1 + m2)) * (glm::dot(v1 - v2, x1 - x2) / glm::distance2(x1, x2)) * (x1 - x2);
}

void ElasticCollision::applyCollision() {
    this->thisObject->parentRigidBody->velocity = newVelocity;
}