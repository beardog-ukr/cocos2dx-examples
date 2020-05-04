#pragma once

#include "cocos2d.h"

class SkeletonPerson : public cocos2d::Sprite {
public:

static bool loadAnimations();
static bool unloadAnimations();

static SkeletonPerson* create(const cocos2d::Vec2& startPos, const cocos2d::Vec2& endPos);

// void start();

protected:
SkeletonPerson();
virtual ~SkeletonPerson();

bool initSelf(const cocos2d::Vec2& startPos, const cocos2d::Vec2& endPos);

void doWalk();

cocos2d::Vec2 positionA;
cocos2d::Vec2 positionB;
bool goingFromAtoB;

};