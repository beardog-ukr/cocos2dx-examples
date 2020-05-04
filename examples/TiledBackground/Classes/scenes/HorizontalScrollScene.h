#pragma once

#include "cocos2d.h"

class HorizontalScrollScene : public cocos2d::Scene {
public:

static cocos2d::Scene* createScene();

virtual bool           init();

// implement the "static create()" method manually
CREATE_FUNC(HorizontalScrollScene);

protected:

bool initBackground();
bool initCamera();

bool initMage();
bool loadMageAnimation();
cocos2d::Sprite* mage;
cocos2d::Vec2 expectedMagePos;

void ensureCameraFollowsMage();
void doMageWalk();

bool initSkeletons();

bool initKeyboardProcessing();
void onKeyPressedScene(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);



};
