---
title: "Рух та інші перетворення об'єктів"
date: 2020-03-29T15:55:25+03:00
draft: false
tags: ["Action", "MoveTo"]
---

Ми вже вміємо обробляти події клавіатури або натискання на кнопки. Тепер треба починати реагувати на ці події та щось зробити з об'єктом.

<!--more-->

Наприклад, ви хочете, щоб після натиснення кнопки спрайт перемістився з одного місця екрану до іншого. Для того, щоб це виглядало саме як пересування, а не телепортація, координати треба змінювати дуже маленькими, але частими порціями, десятки разів на секунду. Вручну це програмувати незручно, тому у Cococ2d зробили клас [Action](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/db/d61/classcocos2d_1_1_action.html) та його нащадки.  Зокрема, для пересування призначений `MoveTo`, використання якого виглядає так:
```cpp
void ActionsDemoScene::sideButtonCallback(Ref *pSender) {
  MoveTo* moveTo = MoveTo::create(2, Vec2(320,240));
  greenUfo->runAction(moveTo);
}
```
Тут greenUfo — спрайт, розміщений на сцені.

`moveBy` — це акція, що рухатиме спрайт. [Фабричний метод](https://uk.wikipedia.org/wiki/%D0%A4%D0%B0%D0%B1%D1%80%D0%B8%D1%87%D0%BD%D0%B8%D0%B9_%D0%BC%D0%B5%D1%82%D0%BE%D0%B4_(%D1%88%D0%B0%D0%B1%D0%BB%D0%BE%D0%BD_%D0%BF%D1%80%D0%BE%D1%94%D0%BA%D1%82%D1%83%D0%B2%D0%B0%D0%BD%D0%BD%D1%8F)) `MoveTo::create` отримує два параметри:
* час у секундах, протягом якого буде виконуватись перетворення
* Vec2 — масив з двох чисел, які є координатами нового положення об'єкту

В результаті після натиснення кнопки зелене НЛО почне рухатись і через дві секунди опиниться у новому положенні. Картинками це зобразити важко, але має бути якось так:

{{< figure src="/cocos2dx-examples/uk/post/basic-actions-img/ufo_move_01.gif" title="Перехід корабля з одного положення в інше" position="center" >}}

# Види перетворень

Cocos2d може виконувати [дуже багато](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/dd/d0d/group__actions.html) різних перетворень:
* [MoveTo](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/de/d42/classcocos2d_1_1_move_to.html) та [MoveBy](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d6/d7c/classcocos2d_1_1_move_by.html) пересувають спрайти
* [RotateTo](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d0/d71/classcocos2d_1_1_rotate_to.html) та [RotateBy](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d0/d28/classcocos2d_1_1_rotate_by.html) повертають об'єкт на заданий кут
* [FadeTo](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d9/de6/classcocos2d_1_1_fade_to.html), [FadeIn](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d0/d0a/classcocos2d_1_1_fade_in.html) та [FadeOut](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/dd/d49/classcocos2d_1_1_fade_out.html) змінюють прозорість (видимість)
* [SkewTo](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d7/d39/classcocos2d_1_1_skew_to.html) та [SkewBy](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d2/d6c/classcocos2d_1_1_skew_by.html) здійснює перетворення, яке важко описати, краще [побачити](https://www.youtube.com/watch?v=X7I-oQYxnnE)

Більшість перетворень має два варіанти: "To" та "By", особливої різниці між ними немає. Просто один отримує чіткі параметри, які описують новий стан об'єкту (координати, кут повороту тощо), а іншому задають величину, на яку треба змінити поточні параметри, щоб отримати новий стан.

# Апроксимація

За помовчанням протягом акції параметри змінюються рівномірно, але це можна змінити. Наприклад, можно повільно змінювати значення на початку часового інтервалу і швидко у кінці — результат буде схожим на зрушення масивного тіла.

Реалізується це за допомогою класів [Easing*](https://docs.cocos2d-x.org/cocos2d-x/v4/en/actions/basic.html#easing) — їх багато на будь-який смак.

Використовуються вони так:
```cpp
MoveBy* bma = MoveBy::create(6, Vec2(360,0));
EaseIn* bea = EaseIn::create(bma, 1.5f);
blueShip->runAction(bea);
```
Другий параметр конструктора _EaseIn_ — це множник з яким буде змінюватись параметри. Чим він більший, тим помітнішими будуть зміни.

Результат модифікаторів [EaseIn](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/dd/dde/classcocos2d_1_1_ease_in.html) та [EaseOut](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/da/d63/classcocos2d_1_1_ease_out.html):
{{< figure src="/cocos2dx-examples/uk/post/basic-actions-img/easing_triple.gif" title="Різна апроксимація" position="center" >}}

Зелений корабель рухається стандартно, синій та червоний — з модифікаціями. У підсумку дистанцію вони проходять за один і той самий час, але по-різному.
