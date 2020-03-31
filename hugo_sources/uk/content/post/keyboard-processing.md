---
title: "Обробка клавіатури"
date: 2020-03-27T19:30:25+03:00
draft: false
tags: ["keyboard processing", "CC_CALLBACK"]
author: beardog-ukr
---

Ще одним прикладом використання макросу `CC_CALLBACK` може бути робота з клавіатурою.

<!--more-->

Для того, щоб додати у програму можливість обробки клавіатури, в `init` сцени треба додати такий код:
```cpp
// keyboard processing
EventListenerKeyboard* sceneKeyboardListener = EventListenerKeyboard::create();
sceneKeyboardListener->onKeyPressed = CC_CALLBACK_2(ActionsDemoScene::onKeyPressedScene, this);
_eventDispatcher->addEventListenerWithSceneGraphPriority(sceneKeyboardListener, this);
```
при цьому сама обробка натискання може виглядати так:
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

Тобто, спочатку створюється об'єкт класу [EventListenerKeyboard](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/df/d2a/classcocos2d_1_1_event_listener_keyboard.html#details). Це такий маленький клас, що об'єднує пару вказівників на функції: `onKeyPressed` та `onKeyPressed`. Для наших потреб досить буде встановити лише один з них. Ми це робимо, використовуючи макрос `CC_CALLBACK_2`, тому що метод, який оброблятиме натискання, отримує два параметри.

Потім ми реєструємо новостворений об'єкт у глобальному [EventDispatcher](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d2/d1b/classcocos2d_1_1_event_dispatcher.html), який у Cocos2d відповідає за обробку подій.

Теоретично, обробник колбеків зовсім не обов'язково має бути методом сцени. Можна було б реалізувати цю функціональність у якому-небудь іншому класі. Просто це досить зручно, тому що у невеликій програмі саме сцена має доступ до всіх об'єктів, що на ній знаходяться.
