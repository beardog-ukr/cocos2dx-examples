#include "ActionsDemoScene.h"

#include  <tgmath.h> // fabs

USING_NS_CC;
using namespace std;

enum ActionTags {
  AT_UFO_ROTATION = 1,
  AT_UFO_MOVING,
  AT_BS_ROTATION,
  AT_BS_MOVE,
  AT_BS_ROTATION_BACK
};

enum GreenUfoMoveDirections {
  GU_MOVE_LEFT = 1,
  GU_MOVE_RIGHT
};

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

  initSpriteCache();

  addBackground();

  addBlueShip();
  addBlueShipControlButtons();
  blueShipNextMoveIsRight = true;

  addGreenUfo();
  addGreenUfoControlButtons();

  return true;
}

bool ActionsDemoScene::initSpriteCache() {
  SpriteFrameCache* sfc = SpriteFrameCache::getInstance();

  vector<string> filenames = {"ui/rightArrow1.png", "ui/rightArrow2.png",
                              "ui/leftArrow1.png", "ui/leftArrow2.png",
                              "ui/Close1.png", "ui/Close2.png"};

  for (string fn: filenames) {
    SpriteFrame* sf = SpriteFrame::create(fn, Rect(0,0,70,70));
    if (sf == nullptr) {
      log("failed to load %s", fn.c_str());
      return false;
    }
    sfc->addSpriteFrame(sf, fn);
  }

  return true;
}

// =============================================================================

bool ActionsDemoScene::addBackground() {
  const char backFilename[] = "backgrounds/background_1.png";

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
  greenUfo->setPosition(Vec2(160*2, 240));

  addChild(greenUfo, 10);

  Sequence* rseq = Sequence::create(RotateBy::create(3, 30),
                                    RotateBy::create(3, -30), nullptr);

  RepeatForever* reps = RepeatForever::create(rseq);
  reps->setTag(AT_UFO_ROTATION);

  greenUfo->runAction(reps);

  return true;
}

// =============================================================================

bool ActionsDemoScene::addGreenUfoControlButtons() {
  MenuItemImage* rightButton = MenuItemImage::create(
    "ui/rightArrow1.png", "ui/rightArrow2.png",
    CC_CALLBACK_1(ActionsDemoScene::ufoMoveCallback, this, GU_MOVE_RIGHT));

  rightButton->setAnchorPoint(Vec2(1,0.5));
  rightButton->setPosition(Vec2(640, 240));

  MenuItemImage* leftButton = MenuItemImage::create(
    "ui/leftArrow1.png", "ui/leftArrow2.png",
    CC_CALLBACK_1(ActionsDemoScene::ufoMoveCallback, this, GU_MOVE_LEFT));

  leftButton->setAnchorPoint(Vec2(0,0.5));
  leftButton->setPosition(Vec2(0, 240));

  Menu* menu = Menu::create(leftButton, rightButton, NULL);
  menu->setPosition(Vec2::ZERO);
  addChild(menu, 1);

  return true;
}

// =============================================================================

bool ActionsDemoScene::addBlueShipControlButtons() {
  SpriteFrameCache* const sfc = SpriteFrameCache::getInstance();

  MenuItemImage* rightButton = MenuItemImage::create();
  rightButton->setNormalSpriteFrame(sfc->getSpriteFrameByName("ui/rightArrow1.png"));
  rightButton->setSelectedSpriteFrame(sfc->getSpriteFrameByName("ui/rightArrow2.png"));
  rightButton->setCallback(CC_CALLBACK_1(ActionsDemoScene::blueShipStartCallback, this));

  rightButton->setAnchorPoint(Vec2(1,0.5));
  rightButton->setPosition(Vec2(640, 120));

  blueShipMoveMII = rightButton;

  MenuItemImage* leftButton = MenuItemImage::create();
  leftButton->setNormalSpriteFrame(sfc->getSpriteFrameByName("ui/Close1.png"));
  leftButton->setSelectedSpriteFrame(sfc->getSpriteFrameByName("ui/Close2.png"));
  leftButton->setCallback( CC_CALLBACK_1(ActionsDemoScene::blueShipStopCallback, this));

  leftButton->setAnchorPoint(Vec2(0,0.5));
  leftButton->setPosition(Vec2(0, 120));

  Menu* menu = Menu::create(leftButton, rightButton, NULL);
  menu->setPosition(Vec2::ZERO);
  addChild(menu, 1);

  return true;
}

// =============================================================================

void ActionsDemoScene::ufoMoveCallback(Ref *pSender, const int direction) {

  const int leftX = 160;
  const int rightX = 160*3;
  const float speed = ((float)160/5);// 160 pixels for 5 seconds
  const Vec2 currPos = greenUfo->getPosition();

  float newX = 0; //undefided at this point
  if (direction == GU_MOVE_RIGHT) {
    newX = rightX;
  }
  else {
    newX = leftX;
  }

  float time = fabs(currPos.x - newX) / speed;

  greenUfo->stopAllActionsByTag(AT_UFO_MOVING);

  MoveBy* moveTo = MoveTo::create(time, Vec2(newX,240));
  moveTo->setTag(AT_UFO_MOVING);
  greenUfo->runAction(moveTo);
}

// =============================================================================

void ActionsDemoScene::blueShipStartCallback(Ref *pSender) {
  if (blueShip->getNumberOfRunningActionsByTag(AT_BS_ROTATION_BACK)>0 ) {
    log("%s: rotating back, do not interfere with moving", __func__);
    return;
  }

  float rotationAngle = 90;
  float newX = 160*3;
  if (!blueShipNextMoveIsRight) {
    rotationAngle = -90;
    newX = 160;
  }

  RotateTo* ra = RotateTo::create(3, rotationAngle);

  const float speed = ((float)160/3);// 160 pixels for 3 seconds
  const Vec2 currPos = blueShip->getPosition();

  float time = fabs(currPos.x - newX) / speed;
  MoveTo* ma = MoveTo::create(time, Vec2(newX, 120));

  CallFunc* finalCf = CallFunc::create([this]() {
    log("%s: here as move finished", __func__);

    RotateTo* ra2 = RotateTo::create(3, 0);
    ra2->setTag(AT_BS_ROTATION_BACK);

    this->blueShip->runAction(ra2);

    string normalSfn = "";
    string selectedSfn = "";
    if (blueShipNextMoveIsRight) {
      blueShipNextMoveIsRight = false;
      normalSfn = "ui/leftArrow1.png";
      selectedSfn = "ui/leftArrow2.png";
    }
    else {
      blueShipNextMoveIsRight = true;
      normalSfn = "ui/rightArrow1.png";
      selectedSfn = "ui/rightArrow2.png";
    }

    SpriteFrameCache* const sfc = SpriteFrameCache::getInstance();
    this->blueShipMoveMII->setNormalSpriteFrame(sfc->getSpriteFrameByName(normalSfn));
    this->blueShipMoveMII->setSelectedSpriteFrame(sfc->getSpriteFrameByName(selectedSfn));
  });

  Sequence* seq = Sequence::create(ra, ma, finalCf, nullptr);
  seq->setTag(AT_BS_MOVE);

  blueShip->runAction(seq);
}

// =============================================================================

void ActionsDemoScene::blueShipStopCallback(Ref *pSender) {
  if (blueShip->getActionByTag(AT_BS_ROTATION_BACK) != nullptr) {
    log("%s: rotating back, do not interfere", __func__);
    return;
  }

  log("%s: here as need to stop", __func__);
  blueShip->stopAllActionsByTag(AT_BS_MOVE);

  RotateTo* ra = RotateTo::create(5, 0);
  ra->setTag(AT_BS_ROTATION_BACK);

  blueShip->runAction(ra);

}

// =============================================================================
