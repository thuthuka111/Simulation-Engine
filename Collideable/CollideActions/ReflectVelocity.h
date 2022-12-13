//
// Created by Thuthuka on 2022/12/09.
//

#ifndef FAKEBASKETBALL_REFLECTVELOCITY_H
#define FAKEBASKETBALL_REFLECTVELOCITY_H

#include "../CollideAction.h"
#include "glm/glm.hpp"

class ReflectVelocity: public CollideAction {
private:
    glm::vec2 newVelocity;
public:
    void reactToCollision(Collider* thisObject, Collider* otherObject);
    void applyCollision();
};


#endif //FAKEBASKETBALL_REFLECTVELOCITY_H
