//
// Created by Thuthuka on 2022/12/05.
//

#ifndef FAKEBASKETBALL_COLLIDEACTION_H
#define FAKEBASKETBALL_COLLIDEACTION_H

class Collider;

class CollideAction {
public:
    virtual void reactToCollision(Collider* thisObject, Collider* otherObject) = 0;
    virtual void applyCollision() = 0;
};


#endif //FAKEBASKETBALL_COLLIDEACTION_H
