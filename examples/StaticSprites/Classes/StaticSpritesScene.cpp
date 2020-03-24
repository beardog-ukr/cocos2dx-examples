#include "StaticSpritesScene.h"

USING_NS_CC;

Scene * StaticSpritesScene::createScene() {
  return StaticSpritesScene::create();
}

//


// on "init" you need to initialize your instance
bool StaticSpritesScene::init() {
  //////////////////////////////
  // 1. super init first
  if (!Scene::init()) {
    return false;
  }

  setBackground();

  addNormalCenteredObj();
  addNormalCorneredObj();
  addRotatedCenter45C();
  addRotatedCorner45C();
  addMixed();
  addMixedRotated();

  return true;
}


bool StaticSpritesScene::setBackground() {
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

bool StaticSpritesScene::addNormalCenteredObj() {
  const char shipFilename[] = "ships/playerShip3_blue.png";

  Sprite* sprite = Sprite::create(shipFilename);

  sprite->setAnchorPoint(Vec2(0.5,0.5));
  sprite->setPosition(Vec2(160, 240));

  addChild(sprite, 10);

  return true;
}

bool StaticSpritesScene::addNormalCorneredObj() {
  const char shipFilename[] = "ships/playerShip3_green.png";
  Sprite* sprite = Sprite::create(shipFilename);

  sprite->setAnchorPoint(Vec2(0,0));
  sprite->setPosition(Vec2(480, 240));

  addChild(sprite, 10);

  return true;
}

bool StaticSpritesScene::addRotatedCenter45C() {
  const char shipFilename[] = "ships/playerShip3_blue.png";
  Sprite* sprite = Sprite::create(shipFilename);

  sprite->setRotation(45);
  sprite->setPosition(Vec2(160, 120));

  addChild(sprite, 10);

  return true;
}


bool StaticSpritesScene::addRotatedCorner45C() {
  const char shipFilename[] = "ships/playerShip3_green.png";
  Sprite* sprite = Sprite::create(shipFilename);

  sprite->setAnchorPoint(Vec2(0,0));
  sprite->setRotation(45);
  sprite->setPosition(Vec2(480, 120));

  addChild(sprite, 10);

  return true;
}

bool StaticSpritesScene::addMixed() {
  Node* mixedShip = prepareMixedShip();

  mixedShip->setPosition(Vec2(320, 240));
  addChild(mixedShip, 10);

  return true;
}

bool StaticSpritesScene::addMixedRotated() {
  Node* mixedShip = prepareMixedShip();

  mixedShip->setPosition(Vec2(320, 120));
  mixedShip->setRotation(45);
  addChild(mixedShip, 10);

  return true;
}

Node* StaticSpritesScene::prepareMixedShip() {
  const char cockpitFilename[] = "parts/cockpitRed.png";
  Sprite* cockpitSprite = Sprite::create(cockpitFilename);

  const char wingFilename[] = "parts/wingRed.png";
  Sprite* rightWingSprite = Sprite::create(wingFilename);
  Sprite* leftWingSprite = Sprite::create(wingFilename);

  Node* resultNode = Node::create();
  resultNode->setAnchorPoint(Vec2(0.5,0.5));

  //
  rightWingSprite->setAnchorPoint(Vec2(0.5,0.5));
  rightWingSprite->setScale(1, -1);
  rightWingSprite->setPosition(Vec2(60, 15));
  rightWingSprite->setRotation(90);
  resultNode->addChild(rightWingSprite);

  leftWingSprite->setAnchorPoint(Vec2(0.5,0.5));
  leftWingSprite->setPosition(Vec2(-60, 15));
  leftWingSprite->setRotation(90);
  resultNode->addChild(leftWingSprite);

  resultNode->addChild(cockpitSprite);
  //
  resultNode->setAnchorPoint(Vec2(0.5,0.5));

  return resultNode;
}


