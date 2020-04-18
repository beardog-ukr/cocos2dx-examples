#pragma once

#include "cocos2d.h"

class InstantMenuScene : public cocos2d::Scene {
public:

/**
 * Creates the instant scene
 *
 * Creates the InstantMenuScene object;
 *
 * @param interruptedScene scene to be interrupted,
 */
static InstantMenuScene* create(cocos2d::Scene          *interruptedScene,
                                cocos2d::RenderTexture *sceneBackground);

CC_CONSTRUCTOR_ACCESS:
InstantMenuScene();
// InstantMenuScene(cocos2d::RenderTexture *sceneBackground);

virtual ~InstantMenuScene();

protected:


bool initWithBackground(cocos2d::RenderTexture *sceneBackground);

bool initMenuBackground();
bool initMenuNode();

void goToPrevious(Ref *pSender);
void exitApp(Ref *pSender);

// MainGameScene *interruptedScene;

// cocos2d::Node* prepareMenuNode();
};
