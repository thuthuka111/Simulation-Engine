
//#include <memory>
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
    platformCircle.setCollisionAction(new ReflectVelocity());
    platformCircle.position.y = -0.75f;
    platformCircle.acceleration.y = 3.3f;

//    //Making rigidBodies
//    auto platform = Rectangle(1.8f, 0.1f);
//    platform.position.y = -0.75f;
    auto smallBall = Ball(0.05f);
    smallBall.setCollisionAction(new ReflectVelocity());
    // smallBall.velocity.x = 0.05f;
    smallBall.acceleration.y = -6.0f;

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
