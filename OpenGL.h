//
// Created by Thuthuka on 2022/11/25.
//

#ifndef FAKEBASKETBALL_OPENGL_H
#define FAKEBASKETBALL_OPENGL_H

#include <map>

#include "GL/glew.h"
#include <GLFW/glfw3.h>
#include "RigidBodies//RigidBody.h"
#include "Collideable/CollisionSandbox.h"
#include "RigidBodies/PhysicsSandbox.h"

class OpenGL {
    struct trianglesIndex {
        unsigned int start;
        unsigned int size;
    };
    int width = 500, height = 500;
    //TODO have a separate class that for things that are displayed
    std::map<RigidBody*, trianglesIndex> rigidBodyToTriangleNum;
    std::vector<RigidBody*> rigidBodies;
    std::vector<float>* vertices;
    PhysicsSandbox* physicsSandbox;
public:
    ~OpenGL();
    OpenGL();
    int start();
    static GLuint linkShaders();
    void addRigidBody(RigidBody*);
    void setPhysicsSandbox(PhysicsSandbox *physicsSandbox);
};


#endif //FAKEBASKETBALL_OPENGL_H
