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

void switchToNewScene(cocos2d::Ref *pSender, const int sceneCode);

// implement the "static create()" method manually
CREATE_FUNC(MenuScene);
};
