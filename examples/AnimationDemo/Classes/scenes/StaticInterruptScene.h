#pragma once

#include "cocos2d.h"

class StaticInterruptScene : public cocos2d::Scene {
public:

static cocos2d::Scene* createScene();

virtual bool           init();


// implement the "static create()" method manually
CREATE_FUNC(StaticInterruptScene);

protected:
bool initBackground();
bool initCandle();
bool initKeyboardProcessing();
bool initUfo();

void onKeyPressedScene(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event  *event);

};
