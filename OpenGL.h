//
// Created by Thuthuka on 2022/11/25.
//

#ifndef FAKEBASKETBALL_OPENGL_H
#define FAKEBASKETBALL_OPENGL_H

#include "GL/glew.h"
#include <GLFW/glfw3.h>

class OpenGL {
    int width = 500, height = 500;
public:
    int start();
    static GLuint linkShaders();
};


#endif //FAKEBASKETBALL_OPENGL_H
