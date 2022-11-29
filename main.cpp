
//#include <memory>
#include "OpenGL.h"
#include "GameObjects/EnvironmentBox.h"

int main() {
    OpenGL* openGlContext = new OpenGL();

    EnvironmentBox* envBox = new EnvironmentBox(0.8f, 0.8f);
    openGlContext->addGameObject(envBox);

    int returnCode = openGlContext->start();
    delete openGlContext;

    return returnCode;
}
