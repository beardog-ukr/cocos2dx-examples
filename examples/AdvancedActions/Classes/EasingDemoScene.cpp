#include "EasingDemoScene.h"

USING_NS_CC;

enum ActionTags {
  AT_UFO_ROTATION = 1,
  AT_UFO_MOVING
};


// =============================================================================

Scene * EasingDemoScene::createScene() {
  return EasingDemoScene::create();
}

// =============================================================================

// on "init" you need to initialize your instance
bool EasingDemoScene::init() {
  //////////////////////////////
  // 1. super init first
  if (!Scene::init()) {
    return false;
  }

  addBackground();
  addShips();
  addShipControlButton();

  return true;
}

// =============================================================================

bool EasingDemoScene::addBackground() {
  const char backFilename[] = "backgrounds/background_1.png";

  Sprite* sprite = Sprite::create(backFilename);
  if (sprite == nullptr) {
    printf("Error while loading: %s\n", backFilename);
    return false;
  }

  sprite->setAnchorPoint(Vec2(0,0));
  sprite->setPosition(0,0);
  addChild(sprite, 0);

  return true;
}

// =============================================================================

bool EasingDemoScene::addShips() {
  blueShip = Sprite::create("ships/playerShip1_blue.png");
  blueShip->setAnchorPoint(Vec2(0.5,0.5));
  blueShip->setPosition(Vec2(80, 300));

  addChild(blueShip, 10);

  greenShip = Sprite::create("ships/playerShip1_green.png");
  greenShip->setAnchorPoint(Vec2(0.5,0.5));
  greenShip->setPosition(Vec2(80, 180));

  addChild(greenShip, 10);

  redShip = Sprite::create("ships/playerShip1_red.png");
  redShip->setAnchorPoint(Vec2(0.5,0.5));
  redShip->setPosition(Vec2(80, 60));

  addChild(redShip, 10);

  return true;
}

// =============================================================================

bool EasingDemoScene::addShipControlButton() {
  MenuItemImage* rightButton = MenuItemImage::create(
    "ui/rightArrow1.png", "ui/rightArrow2.png",
    CC_CALLBACK_1(EasingDemoScene::shipsStartCallback, this));

  rightButton->setAnchorPoint(Vec2(1,0.5));
  rightButton->setPosition(Vec2(640, 240));

  Menu* menu = Menu::create(rightButton, NULL);
  menu->setPosition(Vec2::ZERO);
  addChild(menu, 1);

  return true;
}

// =============================================================================

void EasingDemoScene::shipsStartCallback(Ref *pSender) {
  MoveBy* bma = MoveBy::create(6, Vec2(360,0));
  EaseIn* bea = EaseIn::create(bma, 1.5f);
  blueShip->runAction(bea);

  MoveBy* gma = MoveBy::create(6, Vec2(360,0));
  greenShip->runAction(gma);

  MoveBy* rma = MoveBy::create(6, Vec2(360,0));
  redShip->runAction(EaseOut::create(rma, 1.5f) );
}

// =============================================================================
