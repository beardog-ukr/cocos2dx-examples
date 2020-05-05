#include "HorizontalScrollScene.h"

#include "SkeletonPerson.h"

USING_NS_CC;
using namespace std;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum z_orders {
  ZO_BACKGROUND = 0,
  ZO_ITEMS,
  ZO_MENU
};


const string mageIdleAnimationName = "evil_mage_idle";
const string mageMoveAnimationName = "evil_mage_move";

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

Scene * HorizontalScrollScene::createScene() {
  return HorizontalScrollScene::create();
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void HorizontalScrollScene::doMageWalk() {
  Vec2 newPos = expectedMagePos;
  newPos.x = newPos.x + 50;

  MoveTo* mt = MoveTo::create(2, newPos);
  expectedMagePos = newPos;

  CallFunc* cf = CallFunc::create([this]() {
    printf("calling for mage walk again\n");

    ensureCameraFollowsMage();

    doMageWalk();
  });

  mage->runAction(Sequence::create(mt, cf, nullptr));
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void HorizontalScrollScene::ensureCameraFollowsMage() {
  Camera* camera = getDefaultCamera();
  const Vec3 currentCameraPos = camera->getPosition3D();
  const Size winSize = Director::getInstance()->getWinSize();
  const float posDiff = fabs(currentCameraPos.x - expectedMagePos.x);
  const float posTh = (winSize.width/2.0-150);
  printf("%s: camera status: %4.2f vs %4.2f\n", __func__, posDiff, posTh);
  if (posDiff>posTh) {
    Vec3 newCameraPos = Vec3(expectedMagePos.x, expectedMagePos.y, currentCameraPos.z);
    MoveTo* cmt = MoveTo::create(1, newCameraPos);
    camera->runAction(cmt);
  }
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool HorizontalScrollScene::init() {
  //////////////////////////////
  // 1. super init first
  if (!Scene::init()) {
    return false;
  }

  if (!initBackground()) {
    return false;
  }

  if (!initSkeletons()) {
    return false;
  }

  if (!initMage()) {
    return false;
  }

  if (!initKeyboardProcessing()) {
    return false;
  }

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool HorizontalScrollScene::initBackground() {
  // const char backFilename[] = "longback/longback_1920x360.png";

  // Sprite* sprite = Sprite::create(backFilename);
  // if (sprite == nullptr) {
  //   printf("Error while loading: %s\n", backFilename);
  //   return false;
  // }

  // sprite->setAnchorPoint(Vec2(0,0));
  // sprite->setPosition(0,0);
  // addChild(sprite, 0);

  const char mapFilename[] = "dung/dungeon02.tmx";

  TMXTiledMap* mapNode = TMXTiledMap::create(mapFilename);
  if (mapNode == nullptr) {
    printf("Error while loading: %s\n", mapFilename);
    return false;
  }
  addChild(mapNode, ZO_BACKGROUND);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool HorizontalScrollScene::initKeyboardProcessing() {
  // keyboard processing
  EventListenerKeyboard* sceneKeyboardListener = EventListenerKeyboard::create();
  sceneKeyboardListener->onKeyPressed =
    CC_CALLBACK_2(HorizontalScrollScene::onKeyPressedScene, this);
  _eventDispatcher->addEventListenerWithSceneGraphPriority(sceneKeyboardListener, this);

  Label* label = Label::createWithTTF("Press 'K' to move fire wizard.",
                                      "fonts/Marker Felt.ttf", 16);
  label->setTextColor(Color4B::YELLOW);
  label->setAnchorPoint(Vec2(0,0));
  label->setPosition(Vec2(10,10));
  addChild(label);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool HorizontalScrollScene::initMage() {
  if (!loadMageAnimation()) {
    return false;
  }

  mage = Sprite::create();
  mage->setAnchorPoint(Vec2(0.5,0.5));
  expectedMagePos = Vec2(320,180);
  mage->setPosition(expectedMagePos);
  addChild(mage, ZO_ITEMS);

  Animation* animation = AnimationCache::getInstance()->getAnimation(mageIdleAnimationName);
  Animate* animate = Animate::create(animation);
  mage->runAction(RepeatForever::create(animate));

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool HorizontalScrollScene::initSkeletons() {
  if (!SkeletonPerson::loadAnimations()) {
    return false;
  }

  const int skCount = 4;
  Vec2 skStarts[skCount] = { Vec2(300,116), Vec2(400,296), Vec2(740,200), Vec2(1000,116)};
  Vec2 skEnds[skCount] =   { Vec2(380,116), Vec2(480,296), Vec2(780,116), Vec2(1080,116) };

  for (int i = 0; i<skCount; i++) {
    SkeletonPerson* skp = SkeletonPerson::create(skStarts[i], skEnds[i]);
    addChild(skp, ZO_ITEMS);
  }

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void HorizontalScrollScene::onKeyPressedScene(EventKeyboard::KeyCode keyCode,
                                              Event                 *event) {
  printf("%s: processing key %d pressed\n", __func__, (int)keyCode);

  if (EventKeyboard::KeyCode::KEY_G == keyCode) {
    printf("%s: G was pressed, moving camera\n", __func__);
    Camera* camera = getDefaultCamera();
    camera->setPosition(Vec2(600, 180));
  }
  else if (EventKeyboard::KeyCode::KEY_K == keyCode) {
    printf("%s: K was pressed, start mage walk\n", __func__);

    mage->stopAllActions();
    Animation* animation = AnimationCache::getInstance()->getAnimation(mageMoveAnimationName);
    Animate* animate = Animate::create(animation);
    mage->runAction(RepeatForever::create(animate));

    doMageWalk();
  }
  else if (EventKeyboard::KeyCode::KEY_X == keyCode) {
    printf("%s: Need to get out.\n", __func__);

    // Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();
  }
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool HorizontalScrollScene::loadMageAnimation() {
  SpriteFrameCache* const sfc = SpriteFrameCache::getInstance();

  const string plistFilename = "evil_mage/evil_mage_images.plist";

  sfc->addSpriteFramesWithFile(plistFilename);
  if (!sfc->isSpriteFramesWithFileLoaded(plistFilename)) {
    printf("Error while loading: %s\n", plistFilename.c_str());
    return false;
  }

  const string animationsPlistFN = "evil_mage/evil_mage_animations.plist";

  AnimationCache * const ac = AnimationCache::getInstance();
  ac->addAnimationsWithFile(animationsPlistFN);

  if ((ac->getAnimation(mageIdleAnimationName) == nullptr) ) {
    printf("Error while loading: %s\n", animationsPlistFN.c_str());
    return false;
  }

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
