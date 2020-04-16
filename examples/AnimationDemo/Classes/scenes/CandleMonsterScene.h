#pragma once

#include "cocos2d.h"

class CandleMonsterScene : public cocos2d::Scene {
public:
CandleMonsterScene();
virtual ~CandleMonsterScene();




static cocos2d::Scene* createScene();

virtual bool           init();


// implement the "static create()" method manually
CREATE_FUNC(CandleMonsterScene);

protected:
bool loadCachedImages();

bool initBackground();
bool initButtons();
bool initMonster();

cocos2d::Sprite* monster;

void monsterMoveCallback(cocos2d::Ref *pSender, const int direction);

};
