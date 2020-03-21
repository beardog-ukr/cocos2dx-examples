---
title: "Action: move sprite over screen"
date: 2019-09-21T15:55:25+03:00
draft: false
tags: ["Action", "keyboard processing"]
---

Use actions to move some sprites over the screeen

# Keyboard processing

First, initialize keyboard processing with following code

```
  auto sceneKeyboardListener = EventListenerKeyboard::create();

  sceneKeyboardListener->onKeyPressed = CC_CALLBACK_2(MainGameScene::onKeyPressedScene, this);
  _eventDispatcher->addEventListenerWithSceneGraphPriority(sceneKeyboardListener, this);
```

then process it like

```
void MainGameScene::onKeyPressedScene(EventKeyboard::KeyCode keyCode,  Event *event)
{
  log("%s: processing key %d pressed", __func__, (int)keyCode);

  switch (keyCode) {
  case EventKeyboard::KeyCode::KEY_UP_ARROW:
    do_something_for_up_arrow();
    break;

  // more options here

  default:
    log("%s: key %d will be ignored", __func__, (int)keyCode);
  }
}
```

# Action class

To be done.





