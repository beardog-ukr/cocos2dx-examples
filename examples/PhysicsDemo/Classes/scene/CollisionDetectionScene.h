#pragma once

#include "cocos2d.h"

class CollisionDetectionScene : public cocos2d::Scene
{
public:
  static cocos2d::Scene* createScene();

  virtual bool init();
  // virtual bool initWithPhysics();

  // a selector callback
  void menuCloseCallback(cocos2d::Ref* pSender);

  // implement the "static create()" method manually
  CREATE_FUNC(CollisionDetectionScene);

protected:
  void onKeyPressedScene(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);

  bool initBackground();
  bool initItems();
  bool initKeyboardProcessing();

  bool initGreenShip();
  bool initBlueShip();
  bool initGrayRock();
  bool initBrownRock();

  bool onContactBegin(cocos2d::PhysicsContact& contact);

  void processMoveRequest(cocos2d::EventKeyboard::KeyCode keyCode);
  void processSwitchObj(cocos2d::EventKeyboard::KeyCode keyCode);

  cocos2d::Node* brownRock;
  cocos2d::Node* grayRock;
  cocos2d::Node* blueShip;
  cocos2d::Node* greenShip;

  cocos2d::Node* currentShip;

};


