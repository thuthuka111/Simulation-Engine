
//#include <memory>
#include "OpenGL.h"
#include "RigidBodies/ConcreteRigidBodies.h"
#include "Collideable/CollideActions/CollideActions.h"

int main() {
    auto *openGlContext = new OpenGL();
    auto *physicsSandbox = new PhysicsSandbox();

    //Assigning the Sandboxes, which must be the same thing the same thing
    openGlContext->setPhysicsSandbox(physicsSandbox);

    //Making rigidBodies
    // auto envBox = Rectangle(1.9f, 1.9f);
    auto platform = Rectangle(1.8f, 0.1f);
    platform.position.y = -0.75f;
//    platform.position.y = -0.075f;
    auto bouncingBall = Ball(0.05f);
    bouncingBall.setCollisionAction(new ReflectVelocity());
    bouncingBall.velocity.x = 0.01f;
    bouncingBall.acceleration.y = -0.6f;

    physicsSandbox->addRigidBody(&platform);
    physicsSandbox->addRigidBody(&bouncingBall);

    // openGlContext->addRigidBody(&envBox);
    openGlContext->addRigidBody(&bouncingBall);
    openGlContext->addRigidBody(&platform);

    int returnCode = openGlContext->start();
    delete openGlContext;

    return returnCode;
}
