#include "InstantMenuScene.h"

USING_NS_CC;
using namespace std;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum z_orders {
  ZO_BACKGROUND = 0,
  ZO_MENU_BACKGROUND,
  ZO_MENU
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

InstantMenuScene::InstantMenuScene() {
  printf("%s: here", __func__);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

InstantMenuScene::~InstantMenuScene() {
  // nothing to do here
  printf("%s: here", __func__);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .


bool InstantMenuScene::initWithBackground(cocos2d::RenderTexture *sceneBackground) {
  log("%s: here", __func__);

  const Size visibleSize = Director::getInstance()->getVisibleSize();
  const Vec2 origin      = Director::getInstance()->getVisibleOrigin();

  setContentSize(visibleSize);

  Node *backgroundNode = sceneBackground;

  backgroundNode->setAnchorPoint(Vec2(0, 0));
  backgroundNode->setPosition(Vec2(0, 0));
  addChild(backgroundNode, ZO_BACKGROUND);

  if (!initMenuBackground()) {
    return false;
  }

  if (!initMenuNode()) {
    return false;
  }

  // Node *menuNode = InstantMenuScene::prepareMenuNode();

  // const Vec2 positionActive = Vec2(2 * (visibleSize.width / 3) + origin.x,
  //                                  visibleSize.height / 2 + origin.y);

  // menuNode->setPosition(positionActive);
  // addChild(menuNode);

  // setAnchorPoint(Vec2(0, 0));
  // setPosition(Vec2(0, 0));

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool InstantMenuScene::initMenuBackground() {
  const char backFilename[] = "instant/backbottom.png";

  Sprite* sprite = Sprite::create(backFilename);
  if (sprite == nullptr) {
    printf("Error while loading: %s\n", backFilename);
    return false;
  }

  sprite->setAnchorPoint(Vec2(0,0));
  sprite->setPosition(0,0);
  addChild(sprite, ZO_MENU_BACKGROUND);


  const char portraitFilename[] = "instant/portrait_neutral.png";

  Sprite* portrait = Sprite::create(portraitFilename);
  if (portrait == nullptr) {
    printf("Error while loading: %s\n", portraitFilename);
    return false;
  }

  portrait->setAnchorPoint(Vec2(1,0));
  portrait->setPosition(640,0);
  addChild(portrait, ZO_MENU_BACKGROUND);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

InstantMenuScene * InstantMenuScene::create(cocos2d::Scene *inInterruptedScene,
                                            cocos2d::RenderTexture *sceneBackground) {
  InstantMenuScene *ret = new (std::nothrow) InstantMenuScene();

  if (ret && ret->initWithBackground(sceneBackground)) {
    ret->autorelease();
  }
  else {
    CC_SAFE_DELETE(ret);
  }

  return ret;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool InstantMenuScene::initMenuNode() {
  SpriteFrameCache* sfc = SpriteFrameCache::getInstance();

  Menu* menu = Menu::create();
  menu->setPosition(180,40);

  const int itemsCount = 2;
  string images1[itemsCount] = {"instant/button_1a.png", "instant/button_2a.png"};
  string images2[itemsCount] = {"instant/button_1.png", "instant/button_2.png"};
  ccMenuCallback mcbs[itemsCount] = {CC_CALLBACK_1(InstantMenuScene::goToPrevious, this),
                                     CC_CALLBACK_1(InstantMenuScene::exitApp, this) };


  for (int i = 0; i< itemsCount; i++) {
    MenuItemImage* item = MenuItemImage::create(images1[i], images2[i], mcbs[i]);

    menu->addChild(item);
  }

  menu->alignItemsHorizontally();

  addChild(menu, ZO_MENU);

  const string caption = "CATS: All your base are belong to us.";

  Label* label = Label::createWithTTF(caption, "fonts/monogram.ttf", 32);
  if (label == nullptr) {
    return false;
  }
  label->setTextColor(Color4B::BLUE);
  label->setAnchorPoint(Vec2(0,0.5));
  label->setPosition(10, 100);
  addChild(label, ZO_MENU);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void InstantMenuScene::goToPrevious(Ref *pSender) {
  log("%s: here", __func__);

  Director::getInstance()->popScene();
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void InstantMenuScene::exitApp(Ref *pSender) {
  log("%s: here", __func__);

  Director::getInstance()->end();
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
