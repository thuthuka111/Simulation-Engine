
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

    auto bottomPlatform = Rectangle(1.7f, 0.06f);
    bottomPlatform.isStatic = true;
    bottomPlatform.position.y = -0.75f;

    auto topPlatform = Rectangle(1.7f, 0.06f);
    topPlatform.isStatic = true;
    topPlatform.position.y = 0.75f;

    auto rightPlatform = Rectangle(1.4f, 0.06f);
    rightPlatform.isStatic = true;
    rightPlatform.position.x = 0.75f;
    rightPlatform.position.y = -0.05f;
    rightPlatform.rotation = 1.57f;

    auto leftPlatform = Rectangle(1.4f, 0.06f);
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

    Point topLeft (-0.5f, 0.5f);
    Point bottomRight(0.5f, 0.1f);
    int numXPoints = 6;
    int numYPoints = 3;
    std::vector<std::vector<Ball*>> ballArray;

    glm::vec2 pointDiff = topLeft - bottomRight;
    for(int i = 0; i < numXPoints; i++) {
        std::vector<Ball*> arrayRows;
        for(int j = 0; j < numYPoints; j++) {
            auto *newBall = new Ball(0.05f);
            newBall->setCollisionAction(new ElasticCollision());
            newBall->colour = hexColour{0, 200, 0};
            newBall->mass = 15.0f;
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

    auto heavyBall = Ball(0.025f);
    heavyBall.setCollisionAction(new ElasticCollision());
    heavyBall.colour = hexColour{255, 140, 0};
    heavyBall.mass = 1.0f;
    heavyBall.velocity.x = -16.3f;
    heavyBall.velocity.y = 0.9f;
    // heavyBall.acceleration.y = -0.45f;

    physicsSandbox->addRigidBody(&heavyBall);
    openGlContext->addRigidBody(&heavyBall);


    int returnCode = openGlContext->start();
    delete openGlContext;

    return returnCode;
}
