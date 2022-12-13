//
// Created by Thuthuka on 2022/12/09.
//

#ifndef FAKEBASKETBALL_NOREACTION_H
#define FAKEBASKETBALL_NOREACTION_H

#include "../CollideAction.h"
#include "glm/glm.hpp"

class NoReaction: public CollideAction {
public:
    void reactToCollision(Collider* thisObject, Collider* otherObject);
    void applyCollision();
};


#endif //FAKEBASKETBALL_NOREACTION_H
