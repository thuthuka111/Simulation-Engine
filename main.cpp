
//#include <memory>
#include <iostream>
#include "glm/gtx/rotate_vector.hpp"
#include "OpenGL.h"
#include "RigidBodies/ConcreteRigidBodies.h"
#include "Collideable/CollideActions/CollideActions.h"

int main() {
    auto *openGlContext = new OpenGL();
    auto *physicsSandbox = new PhysicsSandbox();

    //Assigning the Sandboxes, which must be the same thing the same thing
    openGlContext->setPhysicsSandbox(physicsSandbox);


    auto platformTrigger = [](Collider* thisObject, Collider* otherObject) {
        otherObject->parentRigidBody->velocity = glm::vec2 (0.4f, 0.4f);
        glm::vec3 normalVec = glm::vec3(1.0f, 0.0f, 0.0f);

        normalVec = glm::rotate(normalVec, thisObject->parentRigidBody->rotation, glm::vec3(0.0f, 0.0f, 1.0f));
        normalVec = glm::normalize(normalVec);
        auto temp = glm::reflect(glm::vec3(otherObject->parentRigidBody->velocity, 0.0f), normalVec);

        otherObject->parentRigidBody->velocity = glm::vec2 (temp.x, temp.y);
    };

    auto rightPlatform = Rectangle(1.2f, 0.06f);
    rightPlatform.addTrigger(platformTrigger);
    rightPlatform.mass = 2.0f;
    rightPlatform.position.x = 0.5f;
    rightPlatform.position.y = -0.25f;
    rightPlatform.rotation = 0.45f;

    auto leftPlatform = Rectangle(1.0f, 0.06f);
    leftPlatform.addTrigger(platformTrigger);
    leftPlatform.mass = 2.0f;
    leftPlatform.position.x = -0.5f;
    leftPlatform.position.y = -0.25f;
    leftPlatform.rotation = -0.6f;

    physicsSandbox->addRigidBody(&rightPlatform);
    physicsSandbox->addRigidBody(&leftPlatform);

    openGlContext->addRigidBody(&rightPlatform);
    openGlContext->addRigidBody(&leftPlatform);

    Point topLeft (-0.5f, 0.5f);
    Point bottomRight(0.5f, 0.1f);
    int numXPoints = 3;
    int numYPoints = 2;
    std::vector<std::vector<Ball*>> ballArray;

    glm::vec2 pointDiff = topLeft - bottomRight;
    for(int i = 0; i < numXPoints; i++) {
        std::vector<Ball*> arrayRows;
        for(int j = 0; j < numYPoints; j++) {
            auto *newBall = new Ball(0.05f);
            newBall->setCollisionAction(new ElasticCollision());
            newBall->position = Point(topLeft.x - (i * pointDiff.x / numXPoints), topLeft.y - (j * pointDiff.y / numYPoints));
            newBall->acceleration.y = -0.35f;
            arrayRows.push_back(newBall);
        }
        ballArray.push_back(arrayRows);
    }

    for(auto ballRow: ballArray) {
        for(auto ball: ballRow) {
            physicsSandbox->addRigidBody(ball);
            openGlContext->addRigidBody(ball);
        }
    }

    int returnCode = openGlContext->start();
    delete openGlContext;

    return returnCode;
}
