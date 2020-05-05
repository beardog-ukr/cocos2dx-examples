#include "MainAppScene.h"

USING_NS_CC;
using namespace std;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum z_orders {
  ZO_BACKGROUND = 0,
  ZO_ITEMS,
  ZO_MENU
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

Scene * MainAppScene::createScene() {
  return MainAppScene::create();
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// on "init" you need to initialize your instance
bool MainAppScene::init() {
  //////////////////////////////
  // 1. super init first
  if (!Scene::init()) {
    return false;
  }

  if (!initBackground()) {
    return false;
  }

  if (!initInfoLabel()) {
    return false;
  }

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool MainAppScene::initBackground() {
  const char backFilename[] = "backgrounds/gray.png";

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

bool MainAppScene::initInfoLabel() {

  auto visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 origin      = Director::getInstance()->getVisibleOrigin();

  std::ostringstream ss;
  ss << "Visible size: ";
  ss << visibleSize.width << "x" << visibleSize.height;

  Label* label = Label::createWithTTF(ss.str(), "fonts/Marker Felt.ttf", 120);
  label->setTextColor(Color4B::BLUE);
  label->setAnchorPoint(Vec2(0.5,0.5));
  label->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2+140));
  addChild(label);

  ss.str("");
  ss<< "Origin: ";
  ss << origin.x << ":" << origin.y;

  label = Label::createWithTTF(ss.str(), "fonts/Marker Felt.ttf", 120);
  label->setTextColor(Color4B::BLACK);
  label->setAnchorPoint(Vec2(0.5,0.5));
  label->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2 - 140));
  addChild(label);


  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
