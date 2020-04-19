#include "StaticInterruptScene.h"

#include "InstantMenuScene.h"

USING_NS_CC;
using namespace std;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum z_orders {
  ZO_BACKGROUND = 0,
  ZO_ITEMS,
  ZO_MENU
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

Scene * StaticInterruptScene::createScene() {
  return StaticInterruptScene::create();
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
// on "init" you need to initialize your instance
bool StaticInterruptScene::init() {
  //////////////////////////////
  // 1. super init first
  if (!Scene::init()) {
    return false;
  }

  if (!initBackground()) {
    return false;
  }

  if (!initCandle()) {
    return false;
  }

  if (!initUfo()) {
    return false;
  }

  if (!initKeyboardProcessing()) {
    return false;
  }

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool StaticInterruptScene::initBackground() {
  const char backFilename[] = "backgrounds/background_1.png";

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

bool StaticInterruptScene::initCandle() {
  const char candleBack[] = "candle/candle_back.png";

  Sprite* backSprite = Sprite::create(candleBack);
  if (backSprite == nullptr) {
    printf("Error while loading: %s\n", candleBack);
    return false;
  }

  backSprite->setAnchorPoint(Vec2(0.5,0.5));
  backSprite->setPosition(160,240);
  addChild(backSprite, ZO_ITEMS);

  Vector<SpriteFrame*> animFrames;
  animFrames.reserve(3);
  animFrames.pushBack(SpriteFrame::create("candle/SC300-1.png", Rect(0,0,64,64)));
  animFrames.pushBack(SpriteFrame::create("candle/SC300-2.png", Rect(0,0,64,64)));
  animFrames.pushBack(SpriteFrame::create("candle/SC300-3.png", Rect(0,0,64,64)));

  // create the animation out of the frames
  Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.4f);
  Animate* animate = Animate::create(animation);

  // run it and repeat it forever
  Sprite* candleSprite = Sprite::create();
  candleSprite->runAction(RepeatForever::create(animate));

  candleSprite->setAnchorPoint(Vec2(0.5,0.5));
  const Size bss = backSprite->getContentSize();
  candleSprite->setPosition(Vec2(bss.width/2, bss.height/2));

  backSprite->addChild(candleSprite);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool StaticInterruptScene::initKeyboardProcessing() {
  // keyboard processing
  EventListenerKeyboard* sceneKeyboardListener = EventListenerKeyboard::create();
  sceneKeyboardListener->onKeyPressed =
    CC_CALLBACK_2(StaticInterruptScene::onKeyPressedScene, this);
  _eventDispatcher->addEventListenerWithSceneGraphPriority(sceneKeyboardListener, this);

  Label* label = Label::createWithTTF("Press 'space' to see dialog scene",
                                      "fonts/Marker Felt.ttf", 16);
  label->setAnchorPoint(Vec2(0,0));
  label->setPosition(Vec2(10,10));
  addChild(label);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool StaticInterruptScene::initUfo() {
  const char shipFilename[] = "candle/ufoRed.png";

  Sprite* ufo = Sprite::create(shipFilename);

  ufo->setAnchorPoint(Vec2(0.5,0.5));
  ufo->setPosition(Vec2(160*2, 240));

  addChild(ufo, 10);

  Sequence* rseq = Sequence::create(RotateBy::create(3, 45),
                                    RotateBy::create(3, -45), nullptr);

  ufo->runAction(RepeatForever::create(rseq));

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void StaticInterruptScene::onKeyPressedScene(EventKeyboard::KeyCode keyCode,
                                             Event                 *event) {
  printf("%s: processing key %d pressed\n", __func__, (int)keyCode);

  if (keyCode == EventKeyboard::KeyCode::KEY_SPACE) {
    log("%s: Showing interrupt menu.", __func__);

    const Size visibleSize = Director::getInstance()->getVisibleSize();
    RenderTexture *rt      = RenderTexture::create(visibleSize.width,
                                                   visibleSize.height);

    rt->begin();
    this->visit();
    rt->end();
    rt->getSprite()->setAnchorPoint(Vec2(0, 0));

    Scene *ims = InstantMenuScene::create(rt);

    Director::getInstance()->pushScene(ims);
  }
}


// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .



