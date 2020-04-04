#pragma once

#include "cocos2d.h"

class ActionsDemoScene : public cocos2d::Scene {
public:

static cocos2d::Scene* createScene();

virtual bool           init();

// implement the "static create()" method manually
CREATE_FUNC(ActionsDemoScene);

protected:

bool initSpriteCache();
bool addBackground();
bool addBlueShip();
bool addBlueShipControlButtons();
bool addGreenUfo();
bool addGreenUfoControlButtons();

void ufoMoveCallback(cocos2d::Ref *pSender, const int direction);

void blueShipStartCallback(cocos2d::Ref *pSender);
void blueShipStopCallback(cocos2d::Ref *pSender);

cocos2d::Sprite* blueShip;
cocos2d::MenuItemImage* blueShipMoveMII;
bool blueShipNextMoveIsRight;
cocos2d::Sprite* greenUfo;


};
