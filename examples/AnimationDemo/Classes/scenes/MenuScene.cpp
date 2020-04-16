#include "MenuScene.h"
#include "CandleMonsterScene.h"
#include "SkeletonScene.h"
#include "StaticInterruptScene.h"

USING_NS_CC;
using namespace std;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum SceneCode {
  SC_SceneInterrupt = 10,
  SC_CandleMonster,
  SC_Skeleton
};

enum z_orders {
  ZO_BACKGROUND = 0,
  ZO_MENU = 10
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

Scene * MenuScene::createScene() {
  return MenuScene::create();
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
// on "init" you need to initialize your instance
bool MenuScene::init() {
  //////////////////////////////
  // 1. super init first
  if (!Scene::init()) {
    return false;
  }

  initBackground();
  initMenuButtons();

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool MenuScene::initBackground() {
  const char backFilename[] = "backgrounds/paper_background.png";

  Sprite* sprite = Sprite::create(backFilename);
  if (sprite == nullptr) {
    printf("Error while loading: %s\n", backFilename);
    return false;
  }

  sprite->setAnchorPoint(Vec2(0,0));
  sprite->setPosition(0,0);
  addChild(sprite, ZO_BACKGROUND);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool MenuScene::initMenuButtons() {
  SpriteFrameCache* sfc = SpriteFrameCache::getInstance();

  const string uiFilename = "ui/ui.plist";

  sfc->addSpriteFramesWithFile(uiFilename);
  if (!sfc->isSpriteFramesWithFileLoaded(uiFilename)) {
    printf("Error while loading: %s\n", uiFilename.c_str());
    return false;
  }

  Menu* menu = Menu::create();
  menu->setPosition(320,180);

  const int itemsCount = 3;
  string captions[itemsCount] = {"Scene Interrupt", "Candle Monster", "Skeleton"};
  int callbackCodes[itemsCount] = {SC_SceneInterrupt, SC_CandleMonster, SC_Skeleton};

  for (int i = 0; i< itemsCount; i++) {
    MenuItemImage* item = MenuItemImage::create();
    item->setNormalSpriteFrame(sfc->getSpriteFrameByName("button_1.png"));
    item->setSelectedSpriteFrame(sfc->getSpriteFrameByName("button_2.png"));
    item->setCallback(CC_CALLBACK_1(MenuScene::switchToNewScene, this,callbackCodes[i]));

    Label* label = Label::createWithTTF(captions[i], "fonts/Marker Felt.ttf", 24);
    label->setAnchorPoint(Vec2(0.5,0.5));
    const Size itemSize = item->getContentSize();
    label->setPosition(itemSize.width/2, itemSize.height/2);
    item->addChild(label);

    menu->addChild(item);
  }

  addChild(menu, ZO_MENU);

  menu->alignItemsVertically();

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void MenuScene::switchToNewScene(cocos2d::Ref *pSender, const int sceneCode) {
  Scene* newScene = nullptr;

  switch (sceneCode) {
  case SC_SceneInterrupt:
    newScene = StaticInterruptScene::create();
    break;
  case SC_CandleMonster:
    newScene = CandleMonsterScene::create();
    break;
  case SC_Skeleton:
    newScene = SkeletonScene::create();
    break;
  default:
    printf("bad scene code value\n");
    return;
  }

  Director::getInstance()->pushScene(newScene);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

