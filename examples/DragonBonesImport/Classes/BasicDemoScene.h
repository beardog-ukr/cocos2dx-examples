#pragma once

#include "cocos2d.h"

namespace dragonBones {
class CCArmatureDisplay;
}

class BasicDemoScene : public cocos2d::Scene {
public:

static cocos2d::Scene* createScene();

virtual bool           init();

// implement the "static create()" method manually
CREATE_FUNC(BasicDemoScene);

protected:
bool initBackground();
bool initKeyboard();

bool initWizard();
bool initKnight();

void onKeyPressedScene(cocos2d::EventKeyboard::KeyCode keyCode,
                       cocos2d::Event                 *event);

dragonBones::CCArmatureDisplay* wizard;
int currentWeaponIndex;

dragonBones::CCArmatureDisplay* knight;
};
