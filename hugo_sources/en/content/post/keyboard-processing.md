---
title: "Keyboard processing"
date: 2020-09-23T18:30:25+03:00
draft: false
tags: ["keyboard processing", "CC_CALLBACK"]
author: beardog-ukr
---

Keyboard processing can be another example of `CC_CALLBACK` usage.

<!--more-->

If you want to process keyboard you should add following code to scene's `init` method:
```cpp
// keyboard processing
EventListenerKeyboard* sceneKeyboardListener = EventListenerKeyboard::create();
sceneKeyboardListener->onKeyPressed = CC_CALLBACK_2(ActionsDemoScene::onKeyPressedScene, this);
_eventDispatcher->addEventListenerWithSceneGraphPriority(sceneKeyboardListener, this);
```
Now the actual processing looks like this:
```cpp
void ActionsDemoScene::onKeyPressedScene(EventKeyboard::KeyCode keyCode,
                                         Event                 *event) {
  printf("%s: processing key %d pressed\n", __func__, (int)keyCode);

  if (EventKeyboard::KeyCode::KEY_A == keyCode) {
    printf("%s: key 'a' was pressed\n", __func__);
  }
  else {
    printf("%s: failed to process.\n", __func__);
  }
}
```

First, we create an instance of [EventListenerKeyboard](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/df/d2a/classcocos2d_1_1_event_listener_keyboard.html#details). That's a very small class that has tow pointers to functions: `onKeyPressed` and `onKeyReleased`. For now, we'll use only one of them. The method that will process the event will receive two parameters, so we use `CC_CALLBACK_2` macro.

Then we pass newly created object to global [EventDispatcher](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d2/d1b/classcocos2d_1_1_event_dispatcher.html) instance responsible for processing all events.

Actually, there is no need to process keyboard events in the scene method. You can create and register a lot of these methods wherever you need. I just suppose it's a bit better to keep all of them in one place.
