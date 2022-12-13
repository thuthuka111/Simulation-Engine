
//#include <memory>
#include <iostream>
#include "OpenGL.h"
#include "RigidBodies/ConcreteRigidBodies.h"
#include "Collideable/CollideActions/CollideActions.h"

int main() {
    auto *openGlContext = new OpenGL();
    auto *physicsSandbox = new PhysicsSandbox();

    //Assigning the Sandboxes, which must be the same thing the same thing
    openGlContext->setPhysicsSandbox(physicsSandbox);

    // Testing Circle on circle
    auto platformCircle = Ball(0.09f);
    // platformCircle.setCollisionAction(new ElasticCollision());
    platformCircle.addTrigger([](Collider* otherObject) {
        otherObject->parentRigidBody->velocity *= 2;
    });
    // platformCircle.mass = 4.0f;
    platformCircle.position.y = -0.75f;
    // platformCircle.position.x = 0.01f;
    platformCircle.acceleration.y = 0.1f;

//    //Making rigidBodies
//    auto platform = Rectangle(1.8f, 0.1f);
//    platform.position.y = -0.75f;
    auto smallBall = Ball(0.05f);
    smallBall.setCollisionAction(new ElasticCollision());
    smallBall.acceleration.y = -0.3f;

//    auto bigBall = Ball(0.09f);
//    bigBall.setCollisionAction(new ReflectVelocity());
//    bigBall.position.x = 0.33f;
//    bigBall.acceleration.y = -0.5f;

//    physicsSandbox->addRigidBody(&platform);
    physicsSandbox->addRigidBody(&smallBall);
    physicsSandbox->addRigidBody(&platformCircle);
//    physicsSandbox->addRigidBody(&bigBall);

    // openGlContext->addRigidBody(&envBox);
    openGlContext->addRigidBody(&platformCircle);
    openGlContext->addRigidBody(&smallBall);
//    openGlContext->addRigidBody(&platform);
//    openGlContext->addRigidBody(&bigBall);


    int returnCode = openGlContext->start();
    delete openGlContext;

    return returnCode;
}
