#include "SimpleNoScrollScene.h"

USING_NS_CC;
using namespace std;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum z_orders {
  ZO_BACKGROUND = 0,
  ZO_ITEMS,
  ZO_MENU
};

enum map_meta_codes {
  MMC_OBSTACLE = 5,
  MMC_MAGE_START = 10,
  MMC_KNIGHT_START = 20
};

const string mageIdleAnimationName = "mage_idle";
const string knightIdleAnimationName = "knight_idle";
const string knightWalkAnimationName = "knight_walk";

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
SimpleNoScrollScene::SimpleNoScrollScene() {
  mageStartX = 0;
  mageStartY = 0;

  knightStartX = 0;
  knightStartY = 0;

  obstaclesMap = nullptr;
}


// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

SimpleNoScrollScene::~SimpleNoScrollScene() {
  delete[] obstaclesMap;
  //TODO: unload animations
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

Scene * SimpleNoScrollScene::createScene() {
  return SimpleNoScrollScene::create();
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// on "init" you need to initialize your instance
bool SimpleNoScrollScene::init() {
  //////////////////////////////
  // 1. super init first
  if (!Scene::init()) {
    return false;
  }

  if (!loadKnightAnimation()) {
    return false;
  }

  if (!loadMageAnimation()) {
    return false;
  }

  if (!initMap()) {
    return false;
  }

  if (!initMage()) {
    return false;
  }

  if (!initKnight()) {
    return false;
  }

  if (!initKeyboardProcessing()) {
    return false;
  }

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool SimpleNoScrollScene::initKnight() {
  knight = Sprite::create();

  knight->setAnchorPoint(Vec2(0,0));
  knight->setPosition(knightStartX*16,knightStartY*16);
  addChild(knight, ZO_ITEMS);

  Animation* animation = AnimationCache::getInstance()->getAnimation(knightIdleAnimationName);
  knight->runAction(RepeatForever::create(Animate::create(animation)));

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool SimpleNoScrollScene::initKeyboardProcessing() {
  // keyboard processing
  EventListenerKeyboard* sceneKeyboardListener = EventListenerKeyboard::create();
  sceneKeyboardListener->onKeyPressed = CC_CALLBACK_2(SimpleNoScrollScene::onKeyPressedScene, this);
  _eventDispatcher->addEventListenerWithSceneGraphPriority(sceneKeyboardListener, this);

  Label* label = Label::createWithTTF("Press 'G' and knight will walk",
                                      "fonts/Marker Felt.ttf", 16);
  label->setTextColor(Color4B::BLACK);
  label->setAnchorPoint(Vec2(0,0));
  label->setPosition(Vec2(10,10));
  addChild(label);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool SimpleNoScrollScene::initMage() {
  mage = Sprite::create();
  mage->setAnchorPoint(Vec2(0.5,0.5));
  mage->setPosition(mageStartX*16+8,mageStartY*16+8);
  addChild(mage, ZO_ITEMS);

  Animation* animation = AnimationCache::getInstance()->getAnimation(mageIdleAnimationName);
  Animate* animate = Animate::create(animation);
  mage->runAction(RepeatForever::create(animate));

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool SimpleNoScrollScene::initMap() {
  const char mapFilename[] = "beach/beach_map.tmx";

  TMXTiledMap* mapNode = TMXTiledMap::create(mapFilename);
  if (mapNode == nullptr) {
    printf("Error while loading: %s\n", mapFilename);
    return false;
  }
  addChild(mapNode, ZO_BACKGROUND);

  if (!loadMetaInfo(mapNode)) {
    return false;
  }

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool SimpleNoScrollScene::loadKnightAnimation() {
  SpriteFrameCache* const sfc = SpriteFrameCache::getInstance();

  const string plistFilename = "knight/knight_images.plist";

  sfc->addSpriteFramesWithFile(plistFilename);
  if (!sfc->isSpriteFramesWithFileLoaded(plistFilename)) {
    printf("Error while loading: %s\n", plistFilename.c_str());
    return false;
  }

  const string animationsPlistFN = "knight/knight_animations.plist";

  AnimationCache * const ac = AnimationCache::getInstance();
  ac->addAnimationsWithFile(animationsPlistFN);

  if ((ac->getAnimation(knightIdleAnimationName) == nullptr) ||
      (ac->getAnimation(knightWalkAnimationName) == nullptr)) {
    printf("Error while loading: %s\n", animationsPlistFN.c_str());
    return false;
  }

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool SimpleNoScrollScene::loadMetaInfo(TMXTiledMap* const mapNode) {

  const string metaLayerName = "meta";
  TMXLayer *const metaLayer = mapNode->getLayer(metaLayerName);
  if (metaLayer == nullptr) {
    printf("Failed to find %s layer\n", metaLayerName.c_str());
    return false;
  }

  const Size mapSize = mapNode->getMapSize();
  obstaclesMapWidth = mapSize.width;
  obstaclesMapHeight = mapSize.height;
  obstaclesMap = new bool[obstaclesMapWidth*obstaclesMapHeight];
  memset(obstaclesMap, 0, (obstaclesMapWidth*obstaclesMapHeight));


  for (int tileX = 0; tileX < mapSize.width; tileX++) {
    for (int tileY = 0; tileY < mapSize.height; tileY++) {
      const int tileGid = metaLayer->getTileGIDAt(Vec2(tileX, tileY));
      const Value prop    = mapNode->getPropertiesForGID(tileGid);

      if (prop.isNull()) {
        continue;
      }

      const ValueMap vm   = prop.asValueMap();
      const auto frez = vm.find("MetaCode");

      if (frez == vm.end()) {
        continue;
      }

      const int metaCode = frez->second.asInt();

      switch (metaCode) {
      case MMC_OBSTACLE:
        obstaclesMap[obstaclesMapWidth*(obstaclesMapHeight - tileY - 1) + tileX] = true;
        break;

      //     log("%s: additional farmer pos found at %d:%d", __func__, tileX, tileY);
      //   }

      //   farmerPosFound = true;
      //   farmerStartX   = tileX;
      //   farmerStartY   = mapSize.height - tileY - 1;
      //   break;

      case MMC_MAGE_START:
        mageStartX   = tileX;
        mageStartY   = mapSize.height - tileY - 1;

        // printf("%s: mage pos at %d:%d\n", __func__, tileX, tileY);
        // printf("%s: mage recalc at %d:%d\n", __func__, mageStartX, mageStartY);
        break;

      case MMC_KNIGHT_START:
        knightStartX   = tileX;
        knightStartY   = mapSize.height - tileY - 1;

        currentKnightX = knightStartX;
        currentKnightY = knightStartY;
        break;

        // Note there is no suitable default action here
        // default:
      }


    }
  }

  metaLayer->setVisible(false);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool SimpleNoScrollScene::loadMageAnimation() {
  SpriteFrameCache* const sfc = SpriteFrameCache::getInstance();

  const string plistFilename = "mage/mage_idle.plist";

  sfc->addSpriteFramesWithFile(plistFilename);
  if (!sfc->isSpriteFramesWithFileLoaded(plistFilename)) {
    printf("Error while loading: %s\n", plistFilename.c_str());
    return false;
  }

  const string animationsPlistFN = "mage/mage_animations.plist";

  AnimationCache * const ac = AnimationCache::getInstance();
  ac->addAnimationsWithFile(animationsPlistFN);

  if ((ac->getAnimation(mageIdleAnimationName) == nullptr) ) {
    printf("Error while loading: %s\n", animationsPlistFN.c_str());
    return false;
  }

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void SimpleNoScrollScene::onKeyPressedScene(EventKeyboard::KeyCode keyCode,
                                            Event                 *event) {
  printf("%s: processing key %d pressed\n", __func__, (int)keyCode);

  if (EventKeyboard::KeyCode::KEY_G == keyCode) {
    printf("%s: G was pressed, knight walks\n", __func__);
    doKnightWalk();
  }
  else if (EventKeyboard::KeyCode::KEY_X == keyCode) {
    printf("%s: Need to get out.\n", __func__);

    // Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();
  }
}
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void SimpleNoScrollScene::doKnightWalk() {
  printf("%s: here\n", __func__);

  knight->stopAllActions();

  const int nextX = currentKnightX+1;
  if (obstaclesMap[obstaclesMapWidth*(obstaclesMapHeight - currentKnightY - 1) + nextX]) {
    printf("%s: nope, knight will not go\n", __func__);

    Animation* animation = AnimationCache::getInstance()->getAnimation(knightIdleAnimationName);
    knight->runAction(RepeatForever::create(Animate::create(animation)));
    return;
  }

  currentKnightX = nextX;

  MoveTo* mt = MoveTo::create(2, Vec2(currentKnightX*16,currentKnightY*16));

  CallFunc* processMoveFinished = CallFunc::create([this]() {
    printf("%s: processing finished move\n", __func__);
    this->doKnightWalk();
  });

  knight->runAction(Sequence::create(mt, processMoveFinished, nullptr));

  Animation* animation = AnimationCache::getInstance()->getAnimation(knightWalkAnimationName);
  knight->runAction(RepeatForever::create(Animate::create(animation)));
}


// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
