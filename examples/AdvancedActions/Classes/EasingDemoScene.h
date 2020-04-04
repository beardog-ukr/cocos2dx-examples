#pragma once

#include "cocos2d.h"

class EasingDemoScene : public cocos2d::Scene {
public:

static cocos2d::Scene* createScene();

virtual bool           init();

// implement the "static create()" method manually
CREATE_FUNC(EasingDemoScene);

protected:

bool addBackground();
bool addShips();
bool addShipControlButton();

void shipsStartCallback(cocos2d::Ref *pSender);

cocos2d::Sprite* blueShip;
cocos2d::Sprite* greenShip;
cocos2d::Sprite* redShip;


};
