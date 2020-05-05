#pragma once

#include "cocos2d.h"

class MainAppScene : public cocos2d::Scene {
public:

static cocos2d::Scene* createScene();

virtual bool           init();

// implement the "static create()" method manually
CREATE_FUNC(MainAppScene);

protected:

bool initBackground();
bool initInfoLabel();

// bool initKeyboardProcessing();
// void onKeyPressedScene(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);

};
