#include "BasicDemoScene.h"

#include "dragonBones/cocos2dx/CCDragonBonesHeaders.h"


USING_NS_CC;
using namespace std;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

Scene * BasicDemoScene::createScene() {
  return BasicDemoScene::create();
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool BasicDemoScene::init() {
  if (!Scene::init()) {
    return false;
  }

  if (!initBackground()) {
    return false;
  }

  if (!initKnight() ) {
    return false;
  }

  if (!initWizard() ) {
    return false;
  }

  if (!initKeyboard() ) {
    return false;
  }

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool BasicDemoScene::initKeyboard() {
  Label* label = Label::createWithTTF(
    "Press 'G' to switch wizard's weapons, '1', '2', '3' to set knight's weapons\n 'J' or 'K' to make action",
    "fonts/Marker Felt.ttf",
    16);
  label->setAnchorPoint(Vec2(0,0));
  label->setPosition(Vec2(10,10));
  addChild(label);

  // keyboard processing
  EventListenerKeyboard* sceneKeyboardListener = EventListenerKeyboard::create();
  sceneKeyboardListener->onKeyPressed = CC_CALLBACK_2(BasicDemoScene::onKeyPressedScene, this);
  _eventDispatcher->addEventListenerWithSceneGraphPriority(sceneKeyboardListener, this);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool BasicDemoScene::initBackground() {
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

bool BasicDemoScene::initKnight() {
  dragonBones::CCFactory* factory = dragonBones::CCFactory::getFactory();
  factory->loadDragonBonesData("db_export/MagentaKnight_ske.json", "MagentaKnight");
  factory->loadTextureAtlasData("db_export/MagentaKnight_tex.json", "MagentaKnight");

  factory->loadDragonBonesData("db_export/WeaponPack_ske.json", "WeaponPackMK");
  factory->loadTextureAtlasData("db_export/WeaponPack_tex.json", "WeaponPackMK");

  knight = factory->buildArmatureDisplay("KnightArmature", "MagentaKnight");

  knight->setAnchorPoint(Vec2(0.5,0.5));
  knight->setScale(0.33);
  knight->setPosition(480,140);
  addChild(knight, 10);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool BasicDemoScene::initWizard() {
  dragonBones::CCFactory* factory = dragonBones::CCFactory::getFactory();
  factory->loadDragonBonesData("db_export/BlueWizard_ske.json", "BlueWizard");
  factory->loadTextureAtlasData("db_export/BlueWizard_tex.json", "BlueWizard");

  wizard = factory->buildArmatureDisplay("WizardArmature", "BlueWizard");

  wizard->setAnchorPoint(Vec2(0.5,0.5));
  wizard->setScale(0.33);
  wizard->setPosition(160,140);
  addChild(wizard, 10);

  currentWeaponIndex = 1;

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void BasicDemoScene::onKeyPressedScene(EventKeyboard::KeyCode keyCode,
                                       Event                 *event) {
  printf("%s: processing key %d pressed\n", __func__, (int)keyCode);

  if (EventKeyboard::KeyCode::KEY_G == keyCode) {
    printf("%s: G was pressed, change wizard's weapon\n", __func__);
    currentWeaponIndex++;
    if(currentWeaponIndex>=3) { // model has three possible variants for weapon
      currentWeaponIndex = 0;
    }

    wizard->getArmature()->getSlot("Gun")->setDisplayIndex(currentWeaponIndex);
  }
  else if (EventKeyboard::KeyCode::KEY_1 == keyCode) {
    printf("%s: 1 was pressed, set knight's weapon to sword\n", __func__);

    dragonBones::CCFactory* factory = dragonBones::CCFactory::getFactory();
    factory->replaceSlotDisplay("WeaponPackMK", "WeaponPackArmature", "OneHandWeaponSlot",
                                "classic_sword", knight->getArmature()->getSlot("Weapon"));

  }
  else if (EventKeyboard::KeyCode::KEY_2 == keyCode) {
    printf("%s: 2 was pressed, set knight's weapon to curved sword\n", __func__);

    dragonBones::CCFactory* factory = dragonBones::CCFactory::getFactory();
    factory->replaceSlotDisplay("WeaponPackMK", "WeaponPackArmature", "OneHandWeaponSlot",
                                "curved_sword", knight->getArmature()->getSlot("Weapon"));

  }
  else if (EventKeyboard::KeyCode::KEY_3 == keyCode) {
    printf("%s: 3 was pressed, set knight's weapon to morning star\n", __func__);

    dragonBones::CCFactory* factory = dragonBones::CCFactory::getFactory();
    factory->replaceSlotDisplay("WeaponPackMK", "WeaponPackArmature", "OneHandWeaponSlot",
                                "morning_star", knight->getArmature()->getSlot("Weapon"));
  }
  else if (EventKeyboard::KeyCode::KEY_J == keyCode) {
    printf("%s: J was pressed, do some wizard's action\n", __func__);

    wizard->getAnimation()->play("idle01", 1);//1 to play animation once
  }
  else if (EventKeyboard::KeyCode::KEY_K == keyCode) {
    printf("%s: K was pressed, do some knight's action\n", __func__);

    knight->getAnimation()->play("idle01", 1);//
  }
  else if (EventKeyboard::KeyCode::KEY_X == keyCode) {
    printf("%s: Need to get out.\n", __func__);

    // Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();
  }
}

