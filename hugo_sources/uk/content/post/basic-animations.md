---
title: "Анімація"
date: 2020-03-29T17:30:25+03:00
draft: false
tags: ["Animation", "Action"]
author: beardog-ukr
---

Ще одним способом створення "живого" об'єкту є анімація. Художники малюють ряд кадрів, а Cocos2d їх послідовно показує, використовуючи клас [Animation](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d3/dc5/classcocos2d_1_1_animation.html). Робота з ним схожа на інші акції, хоча сам клас має трошки інше походження.

Для прикладу візьмемо анімацію атаки скелету з [безкоштовного паку](https://jesse-m.itch.io/skeleton-pack). Ось кадри:

{{< figure src="/cocos2dx-examples/post/basic-animations-img/attack_tiles.png" title="Атака скелету" position="center" >}}

А так має виглядати результат:
{{< figure src="/cocos2dx-examples/post/basic-animations-img/attack_animation.gif" title="Анімація атаки" position="center" >}}

https://github.com/cocos2d/cocos2d-x/blob/v4/cocos/base/ccMacros.h
