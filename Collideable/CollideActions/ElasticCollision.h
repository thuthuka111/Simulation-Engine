//
// Created by Thuthuka on 2022/12/05.
//

#ifndef FAKEBASKETBALL_ELASTICCOLLISION_H
#define FAKEBASKETBALL_ELASTICCOLLISION_H

#include "glm/glm.hpp"

#include "../CollideAction.h"

class ElasticCollision: public CollideAction {
private:
    glm::vec2 newVelocity;
public:
    void reactToCollision(Collider* thisObject, Collider* otherObject);
    void applyCollision();
};


#endif //FAKEBASKETBALL_ELASTICCOLLISION_H
