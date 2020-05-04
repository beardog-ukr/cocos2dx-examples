#include "Large2dScene.h"

USING_NS_CC;
using namespace std;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum z_orders {
  ZO_BACKGROUND = 0,
  ZO_ITEMS,
  ZO_MENU
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

Scene * Large2dScene::createScene() {
  return Large2dScene::create();
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool Large2dScene::init() {
  //////////////////////////////
  // 1. super init first
  if (!Scene::init()) {
    return false;
  }

  if (!initBackground()) {
    return false;
  }

  if (!initCamera()) {
    return false;
  }

  if (!initKeyboardProcessing()) {
    return false;
  }

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool Large2dScene::initBackground() {
  const char backFilename[] = "backgrounds/large_background.png";

  Sprite* sprite = Sprite::create(backFilename);
  if (sprite == nullptr) {
    printf("Error while loading: %s\n", backFilename);
    return false;
  }

  mapSize = sprite->getContentSize();
  printf("%s: background image size is %4.2f x %4.2f\n", __func__, mapSize.width, mapSize.height);

  sprite->setAnchorPoint(Vec2(0,0));
  sprite->setPosition(0,0);
  addChild(sprite, 0);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool Large2dScene::initKeyboardProcessing() {
  // keyboard processing
  EventListenerKeyboard* sceneKeyboardListener = EventListenerKeyboard::create();
  sceneKeyboardListener->onKeyPressed = CC_CALLBACK_2(Large2dScene::onKeyPressedScene, this);
  _eventDispatcher->addEventListenerWithSceneGraphPriority(sceneKeyboardListener, this);

  Label* label = Label::createWithTTF("Press arrows to move camera.",
                                      "fonts/Marker Felt.ttf", 16);
  label->setTextColor(Color4B::YELLOW);
  label->setAnchorPoint(Vec2(0,0));
  label->setPosition(Vec2(10,10));
  addChild(label);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void Large2dScene::onKeyPressedScene(EventKeyboard::KeyCode keyCode, Event *event) {
  if (EventKeyboard::KeyCode::KEY_LEFT_ARROW == keyCode) {
    moveCamera(-100, 0);
  }
  else if (EventKeyboard::KeyCode::KEY_RIGHT_ARROW == keyCode) {
    moveCamera(100, 0);
  }
  else if (EventKeyboard::KeyCode::KEY_UP_ARROW == keyCode) {
    moveCamera(0, 100);
  }
  else if (EventKeyboard::KeyCode::KEY_DOWN_ARROW == keyCode) {
    moveCamera(0, -100);
  }
  else if (EventKeyboard::KeyCode::KEY_X == keyCode) {
    printf("%s: Need to get out.\n", __func__);

    // Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();
  }
}
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool Large2dScene::initCamera() {
  Size winSize = Director::getInstance()->getWinSize();
  Camera* camera = Camera::createOrthographic(winSize.width, winSize.height, 1, 1000);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void Large2dScene::moveCamera(const int xDiff, const int yDiff) {
  Camera* camera = getDefaultCamera();

  Vec3 currentPos = camera->getPosition3D();
  Vec3 newPos = Vec3(currentPos.x + xDiff, currentPos.y + yDiff, currentPos.z);

  Size winSize = Director::getInstance()->getWinSize();
  const float xBorderLimit = winSize.width/2;
  if (mapSize.width - newPos.x < xBorderLimit) {
    newPos.x = mapSize.width - xBorderLimit;
  }
  if (newPos.x < xBorderLimit) {
    newPos.x = xBorderLimit;
  }

  const float yBorderLimit = winSize.height/2;
  if (mapSize.height - newPos.y < yBorderLimit) {
    newPos.y = mapSize.height - yBorderLimit;
  }
  if (newPos.y < yBorderLimit) {
    newPos.y = yBorderLimit;
  }

  // camera->setPosition3D(newPos);  // simple, but not so good visually
  //                                // it's nicer to move camera slowly with MoveTo
  MoveTo* mt = MoveTo::create(1, newPos);
  camera->runAction(mt);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .