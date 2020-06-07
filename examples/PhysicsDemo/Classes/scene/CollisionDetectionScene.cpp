#include "scene/CollisionDetectionScene.h"

USING_NS_CC;

#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;

static const int categoryMaskRock = 0x02;
static const int categoryMaskShip = 0x01;

enum ItemsTags {
  IT_green_ship,
  IT_blue_ship,
  IT_gray_rock,
  IT_brown_rock
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

Scene* CollisionDetectionScene::createScene() {
  printf("%s: here\n", __func__);
  Scene* result = CollisionDetectionScene::create();
  return result;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
// on "init" you need to initialize your instance
bool CollisionDetectionScene::init() {
  printf("%s: here\n", __func__);
  //////////////////////////////
  // 1. super init first
  if ( !Scene::initWithPhysics() ) {
    return false;
  }

  getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_SHAPE);

  if (!initBackground()) {
    return false;
  }

  if (!initKeyboardProcessing()) {
    return false;
  }

  if (!initItems()) {
    return false;
  }

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool CollisionDetectionScene::initBackground() {
  const char backFilename[] = "backgrounds/background_1.png";

  Sprite* sprite = Sprite::create(backFilename);
  if (sprite == nullptr) {
    printf("Error while loading: %s\n", backFilename);
    return false;
  }

  sprite->setAnchorPoint(Vec2(0,0));
  sprite->setPosition(0,0);
  addChild(sprite, 0);

  Label* label = Label::createWithTTF(
    "Press arrows to move selected object, or '1'..'4' to select object",
    "fonts/Marker Felt.ttf", 16);
  label->setAnchorPoint(Vec2(0,0));
  label->setPosition(Vec2(10,10));
  addChild(label);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool CollisionDetectionScene::initBlueShip() {
  const char blueShipFN[] = "items/ufoBlue.png";
  Sprite* sprite = Sprite::create(blueShipFN);
  if (sprite == nullptr) {
    printf("Error while loading: %s\n", blueShipFN);
    return false;
  }

  sprite->setPosition(160,120);
  addChild(sprite);

  PhysicsBody* physicsBody = PhysicsBody::createBox(sprite->getContentSize(),
                                                    PhysicsMaterial(0.1f, 1.0f, 0.0f));
  physicsBody->setDynamic(false);
  physicsBody->setCategoryBitmask(categoryMaskShip);
  physicsBody->setCollisionBitmask(categoryMaskRock);
  physicsBody->setContactTestBitmask(0xFFFFFFFF);

  sprite->addComponent(physicsBody);

  blueShip = sprite;
  // currentShip = blueShip;

  printf("%s: here\n", __func__);
  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool CollisionDetectionScene::initBrownRock() {
  //
  const char brownRockFN[] = "items/meteorBrown_big2.png";
  Sprite* sprite = Sprite::create(brownRockFN);
  if (sprite == nullptr) {
    printf("Error while loading: %s\n", brownRockFN);
    return false;
  }

  PhysicsBody* physicsBody = PhysicsBody::createCircle(sprite->getContentSize().width/2,
                                                       PhysicsMaterial(0.1f, 1.0f, 0.0f));
  physicsBody->setDynamic(false);
  physicsBody->setCategoryBitmask(categoryMaskRock);
  physicsBody->setCollisionBitmask(categoryMaskShip);
  physicsBody->setContactTestBitmask(0xFFFFFFFF);

  sprite->addComponent(physicsBody);

  sprite->setPosition(160,240);
  addChild(sprite);
  brownRock = sprite;

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool CollisionDetectionScene::initItems() {
  if (!initGreenShip()) {
    return false;
  }
  if (!initBlueShip()) {
    return false;
  }
  printf("%s: here\n", __func__);
  if (!initGrayRock()) {
    return false;
  }
  if (!initBrownRock()) {
    return false;
  }

  printf("%s: here\n", __func__);
  auto contactListener = EventListenerPhysicsContact::create();
  contactListener->onContactBegin = CC_CALLBACK_1(CollisionDetectionScene::onContactBegin, this);
  _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
  printf("%s: here\n", __func__);
  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool CollisionDetectionScene::initGrayRock() {
  //
  const char grayRockFN[] = "items/meteorGrey_big4.png";
  Sprite* sprite = Sprite::create(grayRockFN);
  if (sprite == nullptr) {
    printf("Error while loading: %s\n", grayRockFN);
    return false;
  }

  PhysicsBody* physicsBody = PhysicsBody::createBox(sprite->getContentSize(),
                                                    PhysicsMaterial(0.1f, 1.0f, 0.0f));
  physicsBody->setDynamic(false);
  physicsBody->setCategoryBitmask(categoryMaskRock);
  physicsBody->setCollisionBitmask(categoryMaskShip);
  physicsBody->setContactTestBitmask(0xFFFFFFFF);

  sprite->addComponent(physicsBody);


  sprite->setPosition(480,240);
  addChild(sprite);
  grayRock = sprite;

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool CollisionDetectionScene::initGreenShip() {
  const char greenShipFN[] = "items/playerShip3_green.png";
  Sprite* sprite = Sprite::create(greenShipFN);
  if (sprite == nullptr) {
    printf("Error while loading: %s\n", greenShipFN);
    return false;
  }

  const Vec2 greenPoints[4] = {
    {.x = -49, .y = -25}, {.x = 49, .y = -25},
    {.x = 0, .y = 35}, {.x = 0, .y = -35}
  };
  PhysicsBody* physicsBody = PhysicsBody::createPolygon(greenPoints,4,
                                                        PhysicsMaterial(0.1f, 1.0f, 0.0f));
  physicsBody->setDynamic(false);

  sprite->addComponent(physicsBody);
  sprite->getPhysicsBody()->setCategoryBitmask(categoryMaskShip);
  sprite->getPhysicsBody()->setCollisionBitmask(categoryMaskRock);
  physicsBody->setContactTestBitmask(0xFFFFFFFF);

  sprite->setTag(IT_green_ship);

  sprite->setPosition(480,120);
  addChild(sprite);
  greenShip = sprite;
  currentShip = greenShip;

  printf("%s: here\n", __func__);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool CollisionDetectionScene::initKeyboardProcessing() {
  // keyboard processing
  EventListenerKeyboard* sceneKeyboardListener = EventListenerKeyboard::create();
  sceneKeyboardListener->onKeyPressed = CC_CALLBACK_2(CollisionDetectionScene::onKeyPressedScene,
                                                      this);
  _eventDispatcher->addEventListenerWithSceneGraphPriority(sceneKeyboardListener, this);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool CollisionDetectionScene::onContactBegin(PhysicsContact& contact) {
  printf("%s: here\n", __func__);

  Node* nodeA = contact.getShapeA()->getBody()->getNode();
  Node* nodeB = contact.getShapeB()->getBody()->getNode();

  if ((nodeA->getTag()==IT_green_ship)) {
    printf("%s: node A is green ship\n", __func__);
    nodeA->removeFromParentAndCleanup(true);
    greenShip = nullptr;
  }

  if ((nodeB->getTag()==IT_green_ship)) {
    printf("%s: node B is green ship\n", __func__);
    nodeB->removeFromParentAndCleanup(true);
    greenShip = nullptr;
  }

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void CollisionDetectionScene::onKeyPressedScene(EventKeyboard::KeyCode keyCode,
                                                Event                 *event) {
  printf("%s: processing key %d pressed\n", __func__, (int)keyCode);

  if (EventKeyboard::KeyCode::KEY_SPACE == keyCode) {
    currentShip->stopAllActions();
  }
  else if ((keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW)||
           (keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW)||
           (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW)||
           (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW) ) {
    processMoveRequest(keyCode);
  }
  else if ((keyCode == EventKeyboard::KeyCode::KEY_1)||
           (keyCode == EventKeyboard::KeyCode::KEY_2)||
           (keyCode == EventKeyboard::KeyCode::KEY_3)||
           (keyCode == EventKeyboard::KeyCode::KEY_4) ) {
    processSwitchObj(keyCode);
  }
  else if (EventKeyboard::KeyCode::KEY_X == keyCode) {
    printf("%s: Need to get out.\n", __func__);

    // Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();
  }
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void CollisionDetectionScene::processMoveRequest(cocos2d::EventKeyboard::KeyCode keyCode) {
  currentShip->stopAllActions();

  float xDiff = 0;
  float yDiff = 0;
  const float stepValue = 10;

  switch(keyCode) {
  case EventKeyboard::KeyCode::KEY_UP_ARROW:
    yDiff = stepValue;
    break;

  case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
    yDiff = -stepValue;
    break;
  case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
    xDiff = -stepValue;
    break;

  case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
    xDiff = stepValue;
    break;
    //no default section here
  }

  Vec2 cp = currentShip->getPosition();
  Vec2 np = {.x = cp.x +xDiff, .y = cp.y +yDiff};

  MoveTo* mt = MoveTo::create(1.0, np);
  currentShip->runAction(mt);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void CollisionDetectionScene::processSwitchObj(cocos2d::EventKeyboard::KeyCode keyCode) {
  switch(keyCode) {
  case EventKeyboard::KeyCode::KEY_1:
    currentShip = greenShip;
    break;

  case EventKeyboard::KeyCode::KEY_2:
    currentShip = blueShip;
    break;
  case EventKeyboard::KeyCode::KEY_3:
    currentShip = grayRock;
    break;

  case EventKeyboard::KeyCode::KEY_4:
    currentShip = brownRock;
    break;
  default:
    printf("%s: Bad call\n", __func__);
    return;
  }
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .