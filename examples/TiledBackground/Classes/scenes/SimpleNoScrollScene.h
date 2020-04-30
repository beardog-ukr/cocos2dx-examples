#pragma once

#include "cocos2d.h"

class SimpleNoScrollScene : public cocos2d::Scene {
public:

static cocos2d::Scene* createScene();

virtual bool           init();

// implement the "static create()" method manually
CREATE_FUNC(SimpleNoScrollScene);

protected:
SimpleNoScrollScene();
virtual ~SimpleNoScrollScene();

bool initKeyboardProcessing();
bool initKnight();
bool initMage();
bool initMap();
bool loadMetaInfo(cocos2d::TMXTiledMap* const mapNode);
bool loadMageAnimation();
bool loadKnightAnimation();

void onKeyPressedScene(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);

void doKnightWalk();

cocos2d::Sprite* mage;
int mageStartX;
int mageStartY;

cocos2d::Sprite* knight;
int knightStartX;
int knightStartY;
int currentKnightX;
int currentKnightY;

bool* obstaclesMap;
int obstaclesMapWidth;
int obstaclesMapHeight;
};
