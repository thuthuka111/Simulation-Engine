
//#include <memory>
#include "OpenGL.h"
#include "RigidBodies/ConcreteRigidBodies.h"

int main() {
    OpenGL *openGlContext = new OpenGL();

    //Making rigidBodies
    auto envBox = EnvironmentBox(1.9f, 1.9f);
    auto bouncingBall = Ball(0.4f);

    // openGlContext->addRigidBody(&envBox);
    openGlContext->addRigidBody(&bouncingBall);

    int returnCode = openGlContext->start();
    delete openGlContext;

    return returnCode;
}
