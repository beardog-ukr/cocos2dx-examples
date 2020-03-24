#pragma once

#include "cocos2d.h"

class StaticSpritesScene : public cocos2d::Scene {
public:

static cocos2d::Scene* createScene();

virtual bool           init();


protected:
bool setBackground();
bool addNormalCenteredObj();
bool addNormalCorneredObj();

bool addRotatedCenter45C();
bool addRotatedCorner45C();

bool addMixed();
bool addMixedRotated();

cocos2d::Node* prepareMixedShip();

// implement the "static create()" method manually
CREATE_FUNC(StaticSpritesScene);
};
