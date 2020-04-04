#pragma once

#include "cocos2d.h"

class MenuScene : public cocos2d::Scene {
public:

static cocos2d::Scene* createScene();

virtual bool           init();


protected:
bool initBackground();
bool initMenuButtons();

cocos2d::Node* prepareMixedShip();

void firstMenuCallback(cocos2d::Ref *pSender);
void secondMenuCallback(cocos2d::Ref *pSender);
void thirdMenuCallback(cocos2d::Ref *pSender);

// implement the "static create()" method manually
CREATE_FUNC(MenuScene);
};
