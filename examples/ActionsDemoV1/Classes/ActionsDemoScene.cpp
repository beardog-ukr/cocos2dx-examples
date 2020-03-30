#include "ActionsDemoScene.h"

USING_NS_CC;

#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;

// =============================================================================

Scene * ActionsDemoScene::createScene() {
  return ActionsDemoScene::create();
}

// =============================================================================

// on "init" you need to initialize your instance
bool ActionsDemoScene::init() {
  //////////////////////////////
  // 1. super init first
  if (!Scene::init()) {
    return false;
  }

  addBackground();
  addBlueShip();
  addGreenUfo();
  addButtons();

  // keyboard processing
  EventListenerKeyboard* sceneKeyboardListener = EventListenerKeyboard::create();
  sceneKeyboardListener->onKeyPressed = CC_CALLBACK_2(ActionsDemoScene::onKeyPressedScene, this);
  _eventDispatcher->addEventListenerWithSceneGraphPriority(sceneKeyboardListener, this);

  return true;
}

// =============================================================================

bool ActionsDemoScene::addBackground() {
  const char backFilename[] = "background.png";

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

bool ActionsDemoScene::addBlueShip() {
  const char shipFilename[] = "ships/playerShip3_blue.png";

  blueShip = Sprite::create(shipFilename);

  blueShip->setAnchorPoint(Vec2(0.5,0.5));
  blueShip->setPosition(Vec2(160, 120));

  addChild(blueShip, 10);

  return true;
}

// =============================================================================

bool ActionsDemoScene::addGreenUfo() {
  const char shipFilename[] = "ships/ufoGreen.png";

  greenUfo = Sprite::create(shipFilename);

  greenUfo->setAnchorPoint(Vec2(0.5,0.5));
  greenUfo->setPosition(Vec2(160, 240));

  addChild(greenUfo, 10);

  return true;
}

// =============================================================================

bool ActionsDemoScene::addButtons() {
  MenuItemImage* greenUfoButton = MenuItemImage::create(
    "ui/rightArrow1.png", "ui/rightArrow2.png",
    CC_CALLBACK_1(ActionsDemoScene::greenUfoCallback, this));

  greenUfoButton->setAnchorPoint(Vec2(1,0.5));
  greenUfoButton->setPosition(Vec2(640, 240));

  MenuItemImage* blueShipButton = MenuItemImage::create(
    "ui/rightArrow2.png", "ui/rightArrow1.png",
    CC_CALLBACK_1(ActionsDemoScene::blueShipCallback, this));

  blueShipButton->setAnchorPoint(Vec2(1,0.5));
  blueShipButton->setPosition(Vec2(640, 120));

  auto menu = Menu::create(greenUfoButton, blueShipButton, NULL);
  menu->setPosition(Vec2::ZERO);
  addChild(menu, 1);

  return true;
}

// =============================================================================

void ActionsDemoScene::greenUfoCallback(Ref *pSender) {
  MoveTo* moveTo = MoveTo::create(3, Vec2(320,240));
  greenUfo->runAction(moveTo);
}

// =============================================================================

void ActionsDemoScene::blueShipCallback(Ref *pSender) {
  RotateBy* rotateBy = RotateBy::create(2, 90);

  MoveBy* moveBy = MoveBy::create(4, Vec2(160,0));

  Sequence* seq = Sequence::create(rotateBy, moveBy, nullptr);

  blueShip->runAction(seq);
}

// =============================================================================

void ActionsDemoScene::onKeyPressedScene(EventKeyboard::KeyCode keyCode,
                                         Event                 *event) {
  printf("%s: processing key %d pressed\n", __func__, (int)keyCode);

  if (EventKeyboard::KeyCode::KEY_SPACE == keyCode) {
    printf("%s: Space was pressed (but nothing to do)\n", __func__);
  }
  else if (EventKeyboard::KeyCode::KEY_X == keyCode) {
    printf("%s: Need to get out.\n", __func__);

    // Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();
  }
}
