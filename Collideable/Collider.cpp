//
// Created by Thuthuka on 2022/12/03.
//

#include "Collider.h"
#include "CollideAction.h"
#include "../RigidBodies/RigidBody.h"
#include "CollideActions/CollideActions.h"

#include <algorithm>

Collider::Collider(RigidBody *parentRigidBody, CollisionType collisionType) : parentRigidBody(parentRigidBody), collisionType(collisionType) {
    this->collideAction = new NoReaction();
}

Interval Collider::getXInterval() { //default, should be overridden
    return Interval{parentRigidBody->position.x - 1, parentRigidBody->position.x + 1};
}

bool Collider::isIntersecting(Collider *object2) {
    switch (object2->collisionType) {
        case RECTANGLE:
            return isIntersecting((RectangleCollider *) object2);
        case POINT:
            return isIntersecting((PointCollider *) object2);
        case CIRCLE:
            return isIntersecting((CircleCollider *) object2);
    }
}

void Collider::tryCollision(Collider *otherObject) {
    this->collideAction->reactToCollision(this, otherObject);
}

void Collider::resolveCollision() {
    this->collideAction->applyCollision();
}

bool Collider::CIRCLE_POINT(Point A, Point B, float radius) {
    return ((A.x - B.x) * (A.x - B.x) + (A.y - B.y) * (A.y - B.y)) < radius * radius;
}

// Parallel rectangle represented with 3 points where AB is perpendicular to AC
bool Collider::RECT_POINT(Point A, Point B, Point C, Point M) {
    //https://stackoverflow.com/questions/2752725/finding-whether-a-point-lies-inside-a-rectangle-or-not
    //0 <= dot(AB,AM) <= dot(AB,AB) && 0 <= dot(BC,BM) <= dot(BC,BC)

    Point AB(B.x - A.x, B.y - A.y);
    Point AM(M.x - A.x, M.y - A.y);

    Point BC(C.x - B.x, C.y - B.y);
    Point BM(M.x - B.x, M.y - B.y);

    using namespace glm;
    return 0 <= dot(AB, AM) <= dot(AB, AB) && 0 <= dot(BC, BM) <= dot(BC, BC);
}

bool
Collider::RECT_CIRCLE(Point rectangleCenter, float width, float height, float rectangleRotation, Point circle, float radius) {
    using namespace glm;
    // https://www.geeksforgeeks.org/check-if-any-point-overlaps-the-given-circle-and-rectangle/
    Point lowerLeft = glm::vec2(rectangleCenter - glm::vec2(width / 2, height / 2));

    // Aligning to rectangles reference
    mat2x2 rotateMatrix(cos(rectangleRotation), sin(rectangleRotation), -sin(rectangleRotation),
                        cos(rectangleRotation));
    circle = rotateMatrix * circle;

    // Find the nearest point on the
    // rectangle to the center of
    // the circle
    float dx = circle.x - max(lowerLeft.x, min(circle.x, lowerLeft.x + width));
    float dy = circle.y - max(lowerLeft.y, min(circle.y, lowerLeft.y + height));

    // Find the distance between the
    // nearest point and the center
    // of the circle
    // Distance between 2 points,
    // (x1, y1) & (x2, y2) in
    // 2D Euclidean space is
    // ((x1-x2)**2 + (y1-y2)**2)**0.5
    return (dx * dx + dy * dy) <= radius * radius;
}

bool Collider::RECT_RECT(Rect rectA, float RectARotation, Rect rectB, float RectBRotation) {
    // https://www.gamedev.net/tutorials/_/technical/game-programming/2d-rotated-rectangle-collision-r2604/
    float halfWidth = rectA.width / 2;
    float halfHeight = rectB.height / 2;

    // Anti-clockwise starting from the top right
    Point rectAPoints[4] = {
            Point(rectA.center.x + halfWidth, rectA.center.y + halfHeight),
            Point(rectA.center.x - halfWidth, rectA.center.y + halfHeight),
            Point(rectA.center.x - halfWidth, rectA.center.y - halfHeight),
            Point(rectA.center.x + halfWidth, rectA.center.y - halfHeight),
    };
    Point rectBPoints[4] = {
            Point(rectB.center.x + halfWidth, rectB.center.y + halfHeight),
            Point(rectB.center.x - halfWidth, rectB.center.y + halfHeight),
            Point(rectB.center.x - halfWidth, rectB.center.y - halfHeight),
            Point(rectB.center.x + halfWidth, rectB.center.y - halfHeight),
    };

    // Translate the rectangle Points
    glm::mat2x2 rectARotateMatrix(cos(RectARotation), sin(RectARotation), -sin(RectARotation), cos(RectARotation));
    for (auto rectPoint: rectAPoints) {
        rectPoint = rectARotateMatrix * rectPoint;
    }

    glm::mat2x2 rectBRotateMatrix(cos(RectBRotation), sin(RectBRotation), -sin(RectBRotation), cos(RectBRotation));
    for (auto rectPoint: rectBPoints) {
        rectPoint = rectBRotateMatrix * rectPoint;
    }

    // Defining our projection Axes
    glm::vec2 axes[4] = {
            rectAPoints[0] - rectAPoints[1],
            rectAPoints[0] - rectAPoints[3],
            rectBPoints[1] - rectBPoints[2],
            rectBPoints[1] - rectBPoints[0],
    };

    for (auto axis: axes) {
        std::vector<glm::vec2> axisProjections = {
                glm::dot(rectAPoints[0], axis) / glm::abs(axis * axis) * axis,
                glm::dot(rectAPoints[1], axis) / glm::abs(axis * axis) * axis,
                glm::dot(rectAPoints[2], axis) / glm::abs(axis * axis) * axis,
                glm::dot(rectAPoints[3], axis) / glm::abs(axis * axis) * axis,
                glm::dot(rectBPoints[0], axis) / glm::abs(axis * axis) * axis,
                glm::dot(rectBPoints[1], axis) / glm::abs(axis * axis) * axis,
                glm::dot(rectBPoints[2], axis) / glm::abs(axis * axis) * axis,
                glm::dot(rectBPoints[3], axis) / glm::abs(axis * axis) * axis,
        };

        auto scalarAxisProjections = getSortedProjections(&axisProjections[0], axis);

        //min(B) <= max(A) or max(B) >= max(A)
        if (scalarAxisProjections[4] <= scalarAxisProjections[3] || scalarAxisProjections[7] >= scalarAxisProjections[0])
            continue; // we have overlap keep checking
        return false; // Overlap not possible
    }

    return true;
}

std::vector<float> Collider::getSortedProjections(Point *points, glm::vec2 axis) {
    std::vector<float> scalarValues {
        glm::dot(points[0], axis),
        glm::dot(points[1], axis),
        glm::dot(points[2], axis),
        glm::dot(points[3], axis),
        glm::dot(points[4], axis),
        glm::dot(points[5], axis),
        glm::dot(points[6], axis),
        glm::dot(points[7], axis),
    };

    std::sort(scalarValues.begin(), scalarValues.begin() + 3);
    std::sort(scalarValues.begin() + 4, scalarValues.end());

    return scalarValues;
}

bool Collider::CIRCLE_CIRCLE(Point A, float aRad, Point B, float bRad) {
    return glm::distance(A, B) < aRad + bRad;
}
