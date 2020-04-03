---
title: "Послідовності"
date: 2020-03-29T21:30:25+03:00
draft: false
tags: ["Sequence", "Action"]
author: beardog-ukr
---

Якщо вам потрібно виконати кілька акцій одну за одною, на допомогу прийде клас [Sequence](https://docs.cocos2d-x.org/cocos2d-x/v4/en/actions/sequences.html).

Наприклад, синій корабель за помовчанням створюється "носом доверху". Щоб зобразити його "переліт" направо, спочатку треба зробити поворот на 90 градусів, а вже потім переміщувати.

<!--more-->

Для того, щоб це спрацювало, треба створити акції `RotateTo` та `MoveTo`, а потім об'єднати їх у послідовність:
```cpp
RotateBy* rotateBy = RotateBy::create(2, 90);
MoveBy* moveBy = MoveBy::create(4, Vec2(160,0));

Sequence* seq = Sequence::create(rotateBy, moveBy, nullptr);

blueShip->runAction(seq);
```

Фабричний метод, який створює `Sequence`, може отримувати будь-яке число вказівників на об'єкти-нащадки `Action`, а останнім обов'язково має бути `nullptr`. `Sequence` виконає акції одна за одною, приблизно так:

{{< figure src="/cocos2dx-examples/uk/post/basic-actions-img/blue_ship_move_01.gif" title="Послідовне виконання акцій" position="center" >}}

Також у Cocos2d-x є кілька "службових" акцій, які часто використовуються у побудові послідовностей:
* [DelayTime](https://docs.cocos2d-x.org/api-ref/cplusplus/V3.12/d5/d58/classcocos2d_1_1_delay_time.html) просто чекає певний час.
* [Repeat](https://docs.cocos2d-x.org/api-ref/cplusplus/V3.12/df/d05/classcocos2d_1_1_repeat.html) та [RepeatForever](https://docs.cocos2d-x.org/api-ref/cplusplus/V3.12/d9/d37/classcocos2d_1_1_repeat_forever.html) повторюють задану акцію чи послідовність
* [Spawn](https://docs.cocos2d-x.org/api-ref/cplusplus/V3.12/dc/df1/classcocos2d_1_1_spawn.html) запускає кілька акцій одночасно.
* [RemoveSelf](https://docs.cocos2d-x.org/api-ref/cplusplus/V3.12/d4/d11/classcocos2d_1_1_remove_self.html#details) видаляє об'єкт, щодо якого виконується.
* [CallFunc](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d3/d32/classcocos2d_1_1_call_func.html#details) — викликає функцію, вказівник на яку передали у конструкторі. Таким чином можна виконати код у певний момент у послідовності.
