//
// Created by Thuthuka on 2022/12/03.
//

#ifndef FAKEBASKETBALL_COLLIDER_H
#define FAKEBASKETBALL_COLLIDER_H

#include "glm/glm.hpp"
#include <vector>
#include <functional>

class CollideAction;
class RigidBody;
class RectangleCollider;
class PointCollider;
class CircleCollider;

enum CollisionType {
    RECTANGLE,
    POINT,
    CIRCLE,
};

struct Interval {
    float left;
    float right;
};

typedef glm::vec2 Point;

struct Rect {
    Point center;
    float width;
    float height;
};

class Collider {
    friend RigidBody;
private:
    CollideAction* collideAction = nullptr;
    static std::vector<float> getSortedProjections(Point* points, glm::vec2 axis);
    std::function<void(Collider*, Collider*)> f_trigger = {};
protected:
    static bool RECT_RECT(Rect rectA, float RectARotation, Rect rectB, float RectBRotation);
    static bool RECT_CIRCLE(Point rectangleCenter, float width, float height, float rectangleRotation, Point circle, float radius);
    static bool RECT_POINT(Point A, Point B, Point C, Point M);
    static bool CIRCLE_POINT(Point A, Point B, float r);
    static bool CIRCLE_CIRCLE(Point A, float aRad, Point B, float bRad);
    static Point rotatePoint(Point rotationPoint, float angle, Point PointToRotate);
public:
    RigidBody* parentRigidBody;
    CollisionType collisionType;
    Collider(RigidBody *parentRigidBody, CollisionType collisionType);
    void tryCollision(Collider*);
    void resolveCollision();
    bool isIntersecting(Collider* object2);
    void runTrigger(Collider*);
    virtual Interval getXInterval();
    virtual bool isIntersecting(RectangleCollider* object2) = 0;
    virtual bool isIntersecting(CircleCollider* object2) = 0;
    virtual bool isIntersecting(PointCollider* object2) = 0;
};


#endif //FAKEBASKETBALL_COLLIDER_H
