#pragma once

#include "cocos2d.h"

class MemoryDemoScene : public cocos2d::Scene {
public:

static cocos2d::Scene* createScene();

virtual bool           init();

// implement the "static create()" method manually
CREATE_FUNC(MemoryDemoScene);

protected:
bool addBackground();
bool addInitialShip();

bool addNewShip();

bool addControlButtons();
bool addPoolControlButtons();

cocos2d::Sprite* blueShip;
cocos2d::Sprite* shipInPool;

void processMenuNoDelete(cocos2d::Ref *);
void processMenuRemoveChild(cocos2d::Ref *);
void processMenuRemoveSelf(cocos2d::Ref *);

void processMenuRemoveToPool(cocos2d::Ref *);
void processMenuAddFromPool(cocos2d::Ref *);
void processMenuCreateWithoutAdd(cocos2d::Ref *);

void onKeyPressedScene(cocos2d::EventKeyboard::KeyCode keyCode,
                       cocos2d::Event                 *event);

};
