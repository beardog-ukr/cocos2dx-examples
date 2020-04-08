#include "MenuScene.h"
#include "ActionsDemoScene.h"
#include "EasingDemoScene.h"
#include "MemoryDemoScene.h"

USING_NS_CC;
using namespace std;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum SceneCode {
  SC_Easing = 10,
  SC_ActionsDemo,
  SC_MemoryDemo
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
  Menu* menu = Menu::create();
  menu->setPosition(320,180);

  const int itemsCount = 3;
  string captions[itemsCount] = {"Easing Demo", "Actions Demo", "Memory Management"};
  ccMenuCallback mcbs[itemsCount] = {CC_CALLBACK_1(MenuScene::switchToNewScene, this, SC_Easing),
                                     CC_CALLBACK_1(MenuScene::switchToNewScene, this,
                                                   SC_ActionsDemo),
                                     CC_CALLBACK_1(MenuScene::switchToNewScene, this, SC_MemoryDemo)
  };

  for (int i = 0; i< itemsCount; i++) {
    MenuItemImage* item = MenuItemImage::create(
      "ui/button_1.png", "ui/button_2.png", mcbs[i]);

    Label* label = Label::createWithTTF(captions[i], "fonts/Marker Felt.ttf", 24);
    label->setAnchorPoint(Vec2(0.5,0.5));
    const Size itemSize = item->getContentSize();
    label->setPosition(itemSize.width/2, itemSize.height/2);
    item->addChild(label);

    menu->addChild(item);
  }

  addChild(menu, ZO_MENU);

  menu->alignItemsVertically();

}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void MenuScene::switchToNewScene(cocos2d::Ref *pSender, const int sceneCode) {
  Scene* newScene = nullptr;
  switch (sceneCode) {
  case SC_Easing:
    newScene = EasingDemoScene::create();
    break;
  case SC_ActionsDemo:
    newScene = ActionsDemoScene::create();
    break;
  case SC_MemoryDemo:
    newScene = MemoryDemoScene::create();
    break;
  default:
    printf("bad scene code value\n");
    return;
  }

  Director::getInstance()->pushScene(newScene);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

