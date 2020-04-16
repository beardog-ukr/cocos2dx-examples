#pragma once

#include "cocos2d.h"

class SkeletonScene : public cocos2d::Scene {
public:

static cocos2d::Scene* createScene();

virtual bool           init();


// implement the "static create()" method manually
CREATE_FUNC(SkeletonScene);

protected:
bool initBackground();
bool initKeyboardProcessing();
bool initSkeleton();

bool loadImagesAndAnimation();

cocos2d::Sprite* skeleton;

void onKeyPressedScene(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);

};
