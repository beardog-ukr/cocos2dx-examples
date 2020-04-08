#include "MemoryDemoScene.h"
#include "EnhancedSprite.h"

USING_NS_CC;
using namespace std;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

Scene * MemoryDemoScene::createScene() {
  return MemoryDemoScene::create();
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool MemoryDemoScene::init() {
  if (!Scene::init()) {
    return false;
  }

  addBackground();

  addInitialShip();
  addControlButtons();

  addPoolControlButtons();
  shipInPool = nullptr;

  Label* label = Label::createWithTTF("Press 'Backspace' to get back",
                                      "fonts/Marker Felt.ttf", 16);
  label->setAnchorPoint(Vec2(0,0));
  label->setPosition(Vec2(10,10));
  addChild(label);

  // keyboard processing
  EventListenerKeyboard* sceneKeyboardListener = EventListenerKeyboard::create();
  sceneKeyboardListener->onKeyPressed = CC_CALLBACK_2(MemoryDemoScene::onKeyPressedScene, this);
  _eventDispatcher->addEventListenerWithSceneGraphPriority(sceneKeyboardListener, this);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool MemoryDemoScene::addBackground() {
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

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool MemoryDemoScene::addInitialShip() {
  const char shipFilename[] = "ships/playerShip3_blue.png";

  blueShip = EnhancedSprite::create(shipFilename);

  blueShip->setAnchorPoint(Vec2(0.5,0.5));
  blueShip->setPosition(Vec2(480, 120));
  blueShip->setRotation(90);

  addChild(blueShip, 10);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool MemoryDemoScene::addControlButtons() {
  Menu* menu = Menu::create();

  // set the font for menu with text
  MenuItemFont::setFontSize(20);

  const int itemsCount = 3;
  string captions[itemsCount] = {"No Delete", "RemoveSelf", "RemoveChild"};

  ccMenuCallback mcbs[itemsCount] = {CC_CALLBACK_1(MemoryDemoScene::processMenuNoDelete, this),
                                     CC_CALLBACK_1(MemoryDemoScene::processMenuRemoveSelf, this),
                                     CC_CALLBACK_1(MemoryDemoScene::processMenuRemoveChild, this)
  };

  for (int i = 0; i< itemsCount; i++) {
    MenuItemFont* newItem = MenuItemFont::create(captions[i], mcbs[i]);
    menu->addChild(newItem);
  }

  menu->setPosition(Vec2(560, 300));
  menu->setColor(Color3B(0.5f, 0.0f, 0.0f));
  menu->alignItemsVerticallyWithPadding(10);

  addChild(menu);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool MemoryDemoScene::addPoolControlButtons() {
  Menu* menu = Menu::create();

  // set the font for menu with text
  MenuItemFont::setFontSize(20);

  const int itemsCount = 3;
  string captions[itemsCount] = {"Remove To Pool", "Add From Pool", "Create Without Adding"};

  ccMenuCallback mcbs[itemsCount] = {CC_CALLBACK_1(MemoryDemoScene::processMenuRemoveToPool, this),
                                     CC_CALLBACK_1(MemoryDemoScene::processMenuAddFromPool, this),
                                     CC_CALLBACK_1(MemoryDemoScene::processMenuCreateWithoutAdd,
                                                   this)
  };

  for (int i = 0; i< itemsCount; i++) {
    MenuItemFont* newItem = MenuItemFont::create(captions[i], mcbs[i]);
    menu->addChild(newItem);
  }

  menu->setPosition(Vec2(80, 300));
  menu->setColor(Color3B(0.5f, 0.0f, 0.0f));
  menu->alignItemsVerticallyWithPadding(10);

  addChild(menu);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool MemoryDemoScene::addNewShip() {
  const char shipFilename[] = "ships/playerShip3_blue.png";

  Sprite* newShip = EnhancedSprite::create(shipFilename);

  newShip->setAnchorPoint(Vec2(0.5,0.5));
  newShip->setPosition(Vec2(-160, 120));
  newShip->setRotation(90);

  addChild(newShip, 10);

  MoveTo* ma = MoveTo::create(6, Vec2(480, 120));

  CallFunc* finalCf = CallFunc::create([this, newShip]() {
    log("%s: callback implemented in addNewShip: move finished", __func__);
    this->blueShip = newShip;
  });

  Sequence* seq = Sequence::create(ma, finalCf, nullptr);
  newShip->runAction(seq);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void MemoryDemoScene::processMenuNoDelete(Ref *) {
  log("%s: is here", __func__);

  MoveTo* moveAct = MoveTo::create(5, Vec2(800, 120));
  blueShip->runAction(moveAct);

  addNewShip();
}


// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void MemoryDemoScene::processMenuRemoveChild(Ref *) {
  log("%s: is here", __func__);

  //Remove current with RemoveSelf at the end of the sequence
  MoveTo* moveAct = MoveTo::create(5, Vec2(800, 120));

  Sprite* oldShip = blueShip; //save blueShip pointer because it may get changed by the time
                              // the ship reaches "out of range: destination
  CallFunc* finalCf = CallFunc::create([this, oldShip]() {
    log("%s: callback implemented in processMenuRemoveChild: move finished", __func__);
    this->removeChild(oldShip);
  });

  Sequence* seq = Sequence::create(moveAct, finalCf, nullptr);

  blueShip->runAction(seq);

  addNewShip();
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void MemoryDemoScene::processMenuRemoveSelf(Ref *) {
  log("%s: is here", __func__);

  //Remove current with RemoveSelf at the end of the sequence
  MoveTo* moveAct = MoveTo::create(5, Vec2(800, 120));
  Sequence* seq = Sequence::create(moveAct, RemoveSelf::create(), nullptr);

  blueShip->runAction(seq);

  addNewShip();
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void MemoryDemoScene::onKeyPressedScene(EventKeyboard::KeyCode keyCode,
                                        Event                 *event) {
  printf("%s: processing key %d pressed\n", __func__, (int)keyCode);

  if (EventKeyboard::KeyCode::KEY_BACKSPACE == keyCode) {
    printf("%s: Backspace was pressed, get back from this scene\n", __func__);
    Director::getInstance()->popScene();
  }
  else if (EventKeyboard::KeyCode::KEY_X == keyCode) {
    printf("%s: Need to get out.\n", __func__);

    // Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();
  }
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void MemoryDemoScene::processMenuRemoveToPool(cocos2d::Ref *) {
  log("%s: here", __func__);
  if (shipInPool != nullptr) {
    log("%s: Will not work, please clear pool first", __func__);
    return;
  }

  MoveTo* moveAct = MoveTo::create(5, Vec2(800, 120));

  Sprite* oldShip = blueShip; //save blueShip pointer because it may get changed by the time
                              // the ship reaches "out of range: destination
  CallFunc* finalCf = CallFunc::create([this, oldShip]() {
    log("%s: callback implemented in processMenuRemoveToPool: move finished", __func__);
    this->removeChild(oldShip);

    oldShip->retain();
    this->shipInPool = oldShip;

    if (oldShip == nullptr) {
      log("%s: unexpected null oldShip", __func__);
    }
  });

  Sequence* seq = Sequence::create(moveAct, finalCf, nullptr);

  blueShip->runAction(seq);


  // Add new ship
  addNewShip();
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void MemoryDemoScene::processMenuAddFromPool(cocos2d::Ref *) {
  log("%s: here", __func__);
  if (shipInPool == nullptr) {
    log("%s: Will not work, pool is empty", __func__);
    return;
  }

  //remove current ship
  MoveTo* moveAct = MoveTo::create(5, Vec2(800, 120));
  blueShip->runAction(Sequence::create(moveAct, RemoveSelf::create(), nullptr));


  // set action to move current pool ship as new one
  shipInPool->autorelease();

  shipInPool->setPosition(Vec2(-160, 120));
  addChild(shipInPool, 10);

  MoveTo* ma = MoveTo::create(6, Vec2(480, 120));

  CallFunc* finalCf = CallFunc::create([this]() {
    log("%s: callback implemented in processMenuAddFromPool: move finished", __func__);
    this->blueShip = this->shipInPool;
    this->shipInPool = nullptr;
  });

  Sequence* seq = Sequence::create(ma, finalCf, nullptr);
  shipInPool->runAction(seq);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void MemoryDemoScene::processMenuCreateWithoutAdd(cocos2d::Ref *) {
  log("%s: here", __func__);
  const char shipFilename[] = "ships/playerShip3_blue.png";
  Sprite* newShip = EnhancedSprite::create(shipFilename);
}

