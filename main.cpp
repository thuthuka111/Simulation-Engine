
//#include <memory>
#include <iostream>
#include "glm/gtx/rotate_vector.hpp"
#include "OpenGL.h"
#include "RigidBodies/ConcreteRigidBodies.h"
#include "Collideable/CollideActions/CollideActions.h"
#include "InputHandler/PlayerInputHandler.h"

int main() {
    auto *openGlContext = new OpenGL();
    auto *physicsSandbox = new PhysicsSandbox();

    //Assigning the Sandboxes, which must be the same thing the same thing
    openGlContext->setPhysicsSandbox(physicsSandbox);

    auto platformTrigger = [](Collider *thisObject, Collider *otherObject) {
        otherObject->parentRigidBody->velocity = glm::vec2(0.4f, 0.4f);
        glm::vec3 normalVec = glm::vec3(1.0f, 0.0f, 0.0f);

        normalVec = glm::rotate(normalVec, thisObject->parentRigidBody->rotation, glm::vec3(0.0f, 0.0f, 1.0f));
        normalVec = glm::normalize(normalVec);
        auto temp = glm::reflect(glm::vec3(otherObject->parentRigidBody->velocity, 0.0f), normalVec);

        otherObject->parentRigidBody->velocity = glm::vec2(temp.x, temp.y);
    };

    auto bottomPlatform = Rectangle(1.7f, 0.03f);
    bottomPlatform.isStatic = true;
    bottomPlatform.position.y = -0.75f;

    auto topPlatform = Rectangle(1.7f, 0.03f);
    topPlatform.isStatic = true;
    topPlatform.position.y = 0.75f;

    auto rightPlatform = Rectangle(1.4f, 0.05f);
    rightPlatform.isStatic = true;
    rightPlatform.position.x = 0.75f;
    rightPlatform.position.y = -0.05f;
    rightPlatform.rotation = 1.57f;

    auto leftPlatform = Rectangle(1.4f, 0.05f);
    leftPlatform.isStatic = true;
    leftPlatform.position.x = -0.75f;
    leftPlatform.position.y = -0.05f;
    leftPlatform.rotation = -4.71f;

    physicsSandbox->addRigidBody(&rightPlatform);
    physicsSandbox->addRigidBody(&leftPlatform);
    physicsSandbox->addRigidBody(&bottomPlatform);
    physicsSandbox->addRigidBody(&topPlatform);

    openGlContext->addRigidBody(&rightPlatform);
    openGlContext->addRigidBody(&leftPlatform);
    openGlContext->addRigidBody(&bottomPlatform);
    openGlContext->addRigidBody(&topPlatform);

    auto basketBall = Ball(0.066f);
    basketBall.setCollisionAction(new ElasticCollision());
    basketBall.colour = hexColour{255, 140, 0};
    basketBall.mass = 2.0f;

    rightPlatform.addTrigger([&](Collider *thisObject, Collider *otherObject) {
        if (thisObject == basketBall.collider || otherObject == basketBall.collider) {
            openGlContext->rightSideScore++;
        }
    });
    leftPlatform.addTrigger([&](Collider *thisObject, Collider *otherObject) {
        if (thisObject == basketBall.collider || otherObject == basketBall.collider) {
            openGlContext->leftSideScore++;
        }
    });

    physicsSandbox->addRigidBody(&basketBall);
    openGlContext->addRigidBody(&basketBall);

    auto playerBall = Ball(0.043f);
    playerBall.position.x = -0.5f;
    playerBall.setCollisionAction(new ElasticCollision());
    playerBall.colour = hexColour{100, 130, 175};
    playerBall.mass = 1.4f;

    physicsSandbox->addRigidBody(&playerBall);
    openGlContext->addRigidBody(&playerBall);
    // Set Player 1 input handler
    auto player1handler = PlayerInputHandler(&playerBall);
    openGlContext->addInputHandler(&player1handler);

    int returnCode = openGlContext->start();
    delete openGlContext;

    return returnCode;
}
