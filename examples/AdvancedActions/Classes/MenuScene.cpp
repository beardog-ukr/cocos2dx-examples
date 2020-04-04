#include "MenuScene.h"

USING_NS_CC;
using namespace std;

// =============================================================================

enum z_orders {
  ZO_BACKGROUND = 0,
  ZO_MENU = 10
};

// =============================================================================

Scene * MenuScene::createScene() {
  return MenuScene::create();
}

// =============================================================================
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

// =============================================================================

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

// =============================================================================

bool MenuScene::initMenuButtons() {
  Menu* menu = Menu::create();
  menu->setPosition(320,180);

  const int itemsCount = 3;
  string captions[itemsCount] = {"Demo #1", "Demo #2", "Demo #3"};
  ccMenuCallback mcbs[itemsCount] = {CC_CALLBACK_1(MenuScene::firstMenuCallback, this),
                                     CC_CALLBACK_1(MenuScene::secondMenuCallback, this),
                                     CC_CALLBACK_1(MenuScene::thirdMenuCallback, this)
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

// =============================================================================

void MenuScene::firstMenuCallback(Ref *pSender) {
  printf("menu callback for first menu\n");
}

// =============================================================================

void MenuScene::secondMenuCallback(Ref *pSender) {
  printf("menu callback for second menu\n");
}

// =============================================================================

void MenuScene::thirdMenuCallback(Ref *pSender) {
  printf("menu callback for third menu\n");
}

// =============================================================================


