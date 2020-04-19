#pragma once

#include "cocos2d.h"

class InstantMenuScene : public cocos2d::Scene {
public:

/**
 * Creates the instant scene
 *
 * Creates the InstantMenuScene object;
 *
 * @param sceneBackground image to be used as background
 */
static InstantMenuScene* create(cocos2d::RenderTexture *sceneBackground);

CC_CONSTRUCTOR_ACCESS:
InstantMenuScene();
virtual ~InstantMenuScene();

protected:


bool initWithBackground(cocos2d::RenderTexture *sceneBackground);

bool initMenuBackground();
bool initMenuNode();

void goToPrevious(Ref *pSender);
void exitApp(Ref *pSender);
};
