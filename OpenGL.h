//
// Created by Thuthuka on 2022/11/25.
//

#ifndef FAKEBASKETBALL_OPENGL_H
#define FAKEBASKETBALL_OPENGL_H

#include "GL/glew.h"
#include <GLFW/glfw3.h>
#include "GameObjects/GameObject.h"

class OpenGL {
    int width = 500, height = 500;
    std::vector<GameObject*> gameObjects;
    std::vector<float>* vertices;
public:
    ~OpenGL();
    OpenGL();
    int start();
    static GLuint linkShaders();
    void addGameObject(GameObject*);
};


#endif //FAKEBASKETBALL_OPENGL_H
