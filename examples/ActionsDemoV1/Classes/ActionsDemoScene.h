#pragma once

#include "cocos2d.h"

class ActionsDemoScene : public cocos2d::Scene {
public:

static cocos2d::Scene* createScene();

virtual bool           init();

// implement the "static create()" method manually
CREATE_FUNC(ActionsDemoScene);

protected:
void onKeyPressedScene(cocos2d::EventKeyboard::KeyCode keyCode,
                       cocos2d::Event                 *event);

bool addBackground();

bool addBlueShip();
bool addGreenUfo();

bool addButtons();

void greenUfoCallback(cocos2d::Ref *pSender);
void blueShipCallback(cocos2d::Ref *pSender);

cocos2d::Sprite* blueShip;
cocos2d::Sprite* greenUfo;

};
