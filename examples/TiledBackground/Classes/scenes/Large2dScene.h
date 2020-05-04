#pragma once

#include "cocos2d.h"

class Large2dScene : public cocos2d::Scene {
public:

static cocos2d::Scene* createScene();

virtual bool           init();

// implement the "static create()" method manually
CREATE_FUNC(Large2dScene);

protected:

bool initBackground();
bool initCamera();

bool initKeyboardProcessing();
void onKeyPressedScene(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);

cocos2d::Size mapSize;

void moveCamera(const int xDiff, const int yDiff);

};
