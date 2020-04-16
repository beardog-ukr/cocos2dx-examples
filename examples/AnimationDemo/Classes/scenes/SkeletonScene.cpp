#include "SkeletonScene.h"

USING_NS_CC;
using namespace std;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum z_orders {
  ZO_BACKGROUND = 0,
  ZO_ITEMS,
  ZO_MENU
};


const string attackAnimationName = "skeleton_attack";
const string idleAnimationName = "skeleton_idle";

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

Scene * SkeletonScene::createScene() {
  return SkeletonScene::create();
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool SkeletonScene::init() {
  if (!Scene::init()) {
    return false;
  }

  if (!initBackground()) {
    return false;
  }

  if (!loadImagesAndAnimation()) {
    return false;
  }

  if (!initSkeleton()) {
    return false;
  }

  if (!initKeyboardProcessing()) {
    return false;
  }

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool SkeletonScene::initBackground() {
  const char backFilename[] = "backgrounds/background_2.png";

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

bool SkeletonScene::loadImagesAndAnimation() {
  SpriteFrameCache* const sfc = SpriteFrameCache::getInstance();

  const string plistFilename = "skeleton/skeleton_images.plist";

  sfc->addSpriteFramesWithFile(plistFilename);
  if (!sfc->isSpriteFramesWithFileLoaded(plistFilename)) {
    printf("Error while loading: %s\n", plistFilename.c_str());
    return false;
  }

  const string animationsPlistFN = "skeleton/skeleton_animations.plist";

  AnimationCache * const ac = AnimationCache::getInstance();
  ac->addAnimationsWithFile(animationsPlistFN);

  if ((ac->getAnimation(attackAnimationName) == nullptr) ||
      (ac->getAnimation(idleAnimationName) == nullptr)) {
    return false;
  }

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool SkeletonScene::initSkeleton() {

  skeleton = Sprite::createWithSpriteFrameName("skeleton_idle_00.png");
  if (skeleton == nullptr) {
    printf("Error while loading: skeleton_idle_00.png\n");
    return false;
  }

  skeleton->setAnchorPoint(Vec2(0.5,0.5));
  skeleton->setPosition(120,180);
  addChild(skeleton, ZO_ITEMS);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool SkeletonScene::initKeyboardProcessing() {
  // keyboard processing
  EventListenerKeyboard* sceneKeyboardListener = EventListenerKeyboard::create();
  sceneKeyboardListener->onKeyPressed = CC_CALLBACK_2(SkeletonScene::onKeyPressedScene, this);
  _eventDispatcher->addEventListenerWithSceneGraphPriority(sceneKeyboardListener, this);

  Label* label = Label::createWithTTF("Press 'J' or 'K' to get some actions",
                                      "fonts/Marker Felt.ttf", 16);
  label->setAnchorPoint(Vec2(0,0));
  label->setPosition(Vec2(10,10));
  addChild(label);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void SkeletonScene::onKeyPressedScene(EventKeyboard::KeyCode keyCode,
                                      Event                 *event) {
  printf("%s: processing key %d pressed\n", __func__, (int)keyCode);

  if (EventKeyboard::KeyCode::KEY_J == keyCode) {
    printf("%s: J was pressed, attack\n", __func__);
    Animation* animation = AnimationCache::getInstance()->getAnimation(attackAnimationName);
    Animate* animate = Animate::create(animation);
    skeleton->runAction(animate);
  }
  if (EventKeyboard::KeyCode::KEY_K == keyCode) {
    printf("%s: K was pressed, do idle animation\n", __func__);
    Animation* animation = AnimationCache::getInstance()->getAnimation(idleAnimationName);
    Animate* animate = Animate::create(animation);
    skeleton->runAction(animate);
  }
  else if (EventKeyboard::KeyCode::KEY_X == keyCode) {
    printf("%s: Need to get out.\n", __func__);

    // Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();
  }
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .