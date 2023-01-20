//
// Created by Thuthuka on 2023/01/19.
//

#include <string>
#include "Game.h"
#include <iostream>
#include "../OpenGL.h"
#include "../InputHandler/PlayerInputHandler.h"
#include "../RigidBodies/ConcreteRigidBodies.h"
#include "../Collideable/CollideActions/CollideActions.h"

Game::Game(OpenGL *openGLContext, PhysicsSandbox *physicsSandbox): openGlContext(openGLContext), physicsSandbox(physicsSandbox) {}

int Game::start2BallsGame() {
    // Add Score Counting
    auto scoreTextColor= glm::vec3(0.3, 0.7f, 0.9f);
    // Left Score
    auto* leftSideScoreTextEL = new TextElement{"0", 30.0f, 600.0f, 1.0f, scoreTextColor};
    this->textElements.emplace_back(leftSideScoreTextEL);
    // Right Score
    auto* rightSideScoreTextEl = new TextElement{"0", 750.0f, 600.0f, 1.0f, scoreTextColor};
    this->textElements.emplace_back(rightSideScoreTextEl);

    // Basic moving of a ball
    auto bottomPlatform = Rectangle(1.7f, 0.03f);
    bottomPlatform.isStatic = true;
    bottomPlatform.position.y = -0.75f;

    auto topPlatform = Rectangle(1.7f, 0.03f);
    topPlatform.isStatic = true;
    topPlatform.position.y = 0.75f;

    auto rightPlatform = Rectangle(1.4f, 0.05f);
    rightPlatform.isStatic = true;
    rightPlatform.position.x = 0.75f;
    rightPlatform.position.y = -0.05f;
    rightPlatform.rotation = 1.57f;

    auto leftPlatform = Rectangle(1.4f, 0.05f);
    leftPlatform.isStatic = true;
    leftPlatform.position.x = -0.75f;
    leftPlatform.position.y = -0.05f;
    leftPlatform.rotation = -4.71f;

    physicsSandbox->addRigidBody(&rightPlatform);
    physicsSandbox->addRigidBody(&leftPlatform);
    physicsSandbox->addRigidBody(&bottomPlatform);
    physicsSandbox->addRigidBody(&topPlatform);

    openGlContext->addRigidBody(&rightPlatform);
    openGlContext->addRigidBody(&leftPlatform);
    openGlContext->addRigidBody(&bottomPlatform);
    openGlContext->addRigidBody(&topPlatform);

    auto basketBall = Ball(0.066f);
    basketBall.setCollisionAction(new ElasticCollision());
    basketBall.colour = hexColour{255, 140, 0};
    basketBall.mass = 2.0f;

    rightPlatform.addTrigger([&](Collider *thisObject, Collider *otherObject) {
        if (thisObject == basketBall.collider || otherObject == basketBall.collider) {
            rightSideScore++;
            rightSideScoreTextEl->text = std::to_string(rightSideScore);
        }
    });
    leftPlatform.addTrigger([&](Collider *thisObject, Collider *otherObject) {
        if (thisObject == basketBall.collider || otherObject == basketBall.collider) {
            leftSideScore++;
            leftSideScoreTextEL->text = std::to_string(leftSideScore);
        }
    });

    physicsSandbox->addRigidBody(&basketBall);
    openGlContext->addRigidBody(&basketBall);

    auto playerBall = Ball(0.043f);
    playerBall.position.x = -0.5f;
    playerBall.setCollisionAction(new ElasticCollision());
    playerBall.colour = hexColour{100, 130, 175};
    playerBall.mass = 1.4f;

    physicsSandbox->addRigidBody(&playerBall);
    openGlContext->addRigidBody(&playerBall);
    // Set Player 1 input handler
    auto player1handler = PlayerInputHandler(&playerBall);
    openGlContext->addInputHandler(&player1handler);

    openGlContext->setGame(this);
    return openGlContext->start();
}

std::vector<TextElement*> *Game::getTextElements() {
    return &this->textElements;
}
