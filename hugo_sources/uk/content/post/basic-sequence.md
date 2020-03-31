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

З акціями та їх послідовностями можна ще багато чого цікавого робити: дочасно зупиняти, запускати паралельно, затримувати виконання. Однак про це мова піде уже у наступній статті, а поки що на цьому все, дякую за увагу.
