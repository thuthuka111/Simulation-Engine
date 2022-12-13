//
// Created by Thuthuka on 2022/12/05.
//

#ifndef FAKEBASKETBALL_COLLIDEACTION_H
#define FAKEBASKETBALL_COLLIDEACTION_H

class Collider;

class CollideAction {
protected:
    Collider* thisObject = nullptr;
public://TPDP make it mandatory to have a valid pointer to the parent Object
    virtual void reactToCollision(Collider* thisObject, Collider* otherObject) = 0;
    virtual void applyCollision() = 0;
    virtual ~CollideAction();
};


#endif //FAKEBASKETBALL_COLLIDEACTION_H
