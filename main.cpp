
//#include <memory>
#include "OpenGL.h"
#include "GameObjects/EnvironmentBox.h"

int main() {
    OpenGL* openGlContext = new OpenGL();

    EnvironmentBox* envBox = new EnvironmentBox(1.9f, 1.9f);
    openGlContext->addGameObject(envBox);

    int returnCode = openGlContext->start();
    delete openGlContext;

    return returnCode;
}
