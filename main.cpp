#include "OpenGL.h"

int main() {
    OpenGL* openGlContext = new OpenGL();
    int returnCode = openGlContext->start();
    delete openGlContext;

    return returnCode;
}
