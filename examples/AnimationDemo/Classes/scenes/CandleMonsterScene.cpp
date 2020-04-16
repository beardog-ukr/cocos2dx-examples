#include "CandleMonsterScene.h"

USING_NS_CC;
using namespace std;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum z_orders {
  ZO_BACKGROUND = 0,
  ZO_ITEMS,
  ZO_MENU
};

enum VioletMonsterMoveDirections {
  VM_MOVE_LEFT = 1,
  VM_MOVE_RIGHT
};

static const string VM_ANIM_KEY = "VM_AK";

CandleMonsterScene::CandleMonsterScene() {
  monster = nullptr;
}
CandleMonsterScene::~CandleMonsterScene() {
  SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(
    "littleCandleMonster/littleCandleMonster_violet.plist");
  printf("removed unused sprite frames\n");
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

Scene * CandleMonsterScene::createScene() {
  return CandleMonsterScene::create();
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
// on "init" you need to initialize your instance
bool CandleMonsterScene::init() {
  //////////////////////////////
  // 1. super init first
  if (!Scene::init()) {
    return false;
  }

  if (!loadCachedImages()) {
    return false;
  }

  if (!initBackground()) {
    return false;
  }

  if (!initButtons()) {
    return false;
  }


  if (!initMonster()) {
    return false;
  }

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool CandleMonsterScene::loadCachedImages() {
  SpriteFrameCache* sfc = SpriteFrameCache::getInstance();

  const string violetMonsterFN = "littleCandleMonster/littleCandleMonster_violet.plist";
  sfc->addSpriteFramesWithFile(violetMonsterFN);
  if (!sfc->isSpriteFramesWithFileLoaded(violetMonsterFN)) {
    printf("Error while loading: %s\n", violetMonsterFN.c_str());
    return false;
  }

  Vector<SpriteFrame*> animFrames;
  Animation *monsterAnimation = Animation::create();
  char tmps[256];
  for (int i = 1; i<=4; i++) {
    sprintf(tmps, "lm-1-%i.png", i);
    SpriteFrame *sf = sfc->getSpriteFrameByName(tmps);

    if (sf != nullptr) {
      monsterAnimation->addSpriteFrame(sf);
    }
    else {
      printf("Error while loading: %s\n", tmps);
      return false;
    }
  }

  monsterAnimation->setDelayPerUnit(0.1);
  AnimationCache::getInstance()->addAnimation(monsterAnimation, VM_ANIM_KEY);

  // finally
  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool CandleMonsterScene::initBackground() {
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

bool CandleMonsterScene::initButtons() {
  SpriteFrameCache* sfc = SpriteFrameCache::getInstance();

  MenuItemImage* rightButton = MenuItemImage::create();
  rightButton->setNormalSpriteFrame(sfc->getSpriteFrameByName("rightArrow1.png"));
  rightButton->setSelectedSpriteFrame(sfc->getSpriteFrameByName("rightArrow2.png"));
  rightButton->setCallback(CC_CALLBACK_1(CandleMonsterScene::monsterMoveCallback, this,
                                         VM_MOVE_RIGHT));

  rightButton->setAnchorPoint(Vec2(1,0.5));
  rightButton->setPosition(Vec2(640, 240));

  MenuItemImage* leftButton = MenuItemImage::create();
  leftButton->setNormalSpriteFrame(sfc->getSpriteFrameByName("leftArrow1.png"));
  leftButton->setSelectedSpriteFrame(sfc->getSpriteFrameByName("leftArrow2.png"));
  leftButton->setCallback(CC_CALLBACK_1(CandleMonsterScene::monsterMoveCallback, this,
                                        VM_MOVE_LEFT));

  leftButton->setAnchorPoint(Vec2(0,0.5));
  leftButton->setPosition(Vec2(0, 240));

  Menu* menu = Menu::create(leftButton, rightButton, NULL);
  menu->setPosition(Vec2::ZERO);
  addChild(menu, 1);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool CandleMonsterScene::initMonster() {
  monster = Sprite::createWithSpriteFrameName("lm-1-1.png");

  monster->setAnchorPoint(Vec2(0.5,0.5));
  monster->setPosition(Vec2(160*2, 240));

  addChild(monster, 10);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void CandleMonsterScene::monsterMoveCallback(Ref *pSender, const int direction) {
  monster->stopAllActions();

  const int leftX = 160;
  const int rightX = 160*3;
  const float speed = ((float)160/5);// 160 pixels for 5 seconds
  const Vec2 currPos = monster->getPosition();

  float newX = 0; //undefided at this point
  float scaleX = 1;
  if (direction == VM_MOVE_RIGHT) {
    newX = rightX;
    scaleX = -1;
  }
  else {
    newX = leftX;
    scaleX = 1;
  }

  float time = fabs(currPos.x - newX) / speed;

  monster->setScaleX(scaleX);

  Animation* animation = AnimationCache::getInstance()->getAnimation(VM_ANIM_KEY);
  Animate* animate = Animate::create(animation);
  RepeatForever* ra =  RepeatForever::create(animate);

  CallFunc* moveFinalCf = CallFunc::create([this, ra]() {
    log("%s: here as move finished", __func__);
    this->monster->stopAction(ra);
  });

  MoveBy* moveTo = MoveTo::create(time, Vec2(newX,240));

  monster->runAction(Sequence::create(moveTo, moveFinalCf, nullptr));
  monster->runAction(ra);
}


// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

