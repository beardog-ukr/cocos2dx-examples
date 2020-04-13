---
title: "Складніша робота з CC_CALLBACK"
date: 2020-04-03T13:30:25+03:00
draft: false
tags: ["callbacks", "events", "CC_CALLBACK"]
author: beardog-ukr
---

У попередній [статті про колбеки]({{< ref "events-callbacks.md" >}}) було сказано, що цифра у назві макросу `CC_CALLBACK_` задає кількість параметрів, які отримає відповідна функція. Це не зовсім правильно, магія C++ дозволяє зробити з колбеком ще один цікавий фокус.

<!--more-->

Візьмемо для прикладу зелене НЛО з [цього посту]({{< ref "advanced-actions.md" >}}). Переміщення вправо відрізняється від переміщення вліво лише значенням координати, всі інші дії співпадають. Очевидно, було б непогано, якби натиснення на праву та ліву кнопку обробляв один метод, викликаний з різними параметрами. Як цього можна досягти?

Насправді макрос `CC_CALLBACK_` може отримувати будь-яку кількість параметрів. Всі ці параметри [std::bind](https://en.cppreference.com/w/cpp/utility/functional/bind) передасть у функцію, яка буде викликана.

Краще видно на прикладі. Універсальний колбек для обробки натиснення на кнопку оголошується так:
```cpp
class ActionsDemoScene : public cocos2d::Scene {
public:
  // .....
protected:
  void ufoMoveCallback(cocos2d::Ref *pSender, const int direction);
};
```

Тут `direction` — це той самий параметр, який ми хочемо додати до виклику фунції.

А ось так виглядатиме встановлення колбеку:
```cpp
MenuItemImage* leftButton = MenuItemImage::create(
    "ui/leftArrow1.png", "ui/leftArrow2.png",
    CC_CALLBACK_1(ActionsDemoScene::ufoMoveCallback, this, GU_MOVE_LEFT));

MenuItemImage* rightButton = MenuItemImage::create(
  "ui/rightArrow1.png", "ui/rightArrow2.png",
  CC_CALLBACK_1(ActionsDemoScene::ufoMoveCallback, this, GU_MOVE_RIGHT));
```
Тут `GU_MOVE_LEFT` та `GU_MOVE_RIGHT` — це константи, які відрізняють два випадки виклику методу.

Відповідно, у реалізації універсального методу може бути щось таке:
```cpp
void ActionsDemoScene::ufoMoveCallback(Ref *pSender, const int direction) {
  float newX = 0; //undefided at this point
  if (direction == GU_MOVE_RIGHT) {
    newX = rightX;
  }
  else {
    newX = leftX;
  }

  // використання відповідного значення newX
}
```

В принципі, у даному конкретному прикладі можна було б передавати координату прямо під час створення кнопки, і тоді б `if` у методі був би зовсім непотрібним. Проте мені така реалізація здалась не дуже наочною, тому я тут використав додатковий параметр `direction` просто у якості індикатору.
