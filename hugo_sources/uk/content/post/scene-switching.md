---
title: "Перехід між сценами"
date: 2020-04-18T17:30:25+03:00
draft: false
tags: ["Scene", "Director"]
author: beardog-ukr
---

Розробники Cocos2d хотіли, щоб гра зсередини нагадувала театральну виставу: ряд сцен, що змінюються за командою режисера([Director](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d7/df3/classcocos2d_1_1_director.html))

<!--more-->

Власне, для зміни сцен у Director є такі методи:
* `replaceScene()` замінює поточну сцену на нову
* `popScene()`/`pushScene()` працюють зі стеком сцен. _push_ ставить поточну сцену на паузу та зберігає на стеку, потім запускає нову. Відповідно, _pop_ видаляє поточну сцену та забирає зі стеку старішу.
* `popToRootScene()` та `popToSceneStackLevel(int level)` реалізують особливі випадки

У будь-якому випадку, нова сцена повністю закриває те, що було відображено до її запуску. Зазвичай така поведінка є нормальною, але можливі винятки. Припустимо, ви хочете відобразити якесь повідомлення, або просто поставити гру на паузу. Щоб затримка була більш очевидною для гравця, буде краще, якщо події нової сцени відбуватимуться на фоні старої.

Наприклад, ось такий перехід:

{{< figure src="/cocos2dx-examples/uk/post/scene-switching/gray_to_bases.png" title="" position="center" >}}

Спочатку тут була якась сцена з [анімацією-свічкою]({{< ref "basic-animations.md" >}}) та об'єктом, що рухається. Потім це змінюється на сцену з повідомленням та меню, але ця нова сцена має містити старі елементи.

У _Cocos2d_ немає прямого засобу, який би дозволяв досягти такого ефекту. Проте, можна зробити два кроки: зберегти зображення, згенероване для старої сцени, і використати його як фон у новій.

Приблизно так:
```cpp
void StaticInterruptScene::switchToInstantScene() {
    const Size visibleSize = Director::getInstance()->getVisibleSize();
    RenderTexture* rt      = RenderTexture::create(visibleSize.width,
                                                   visibleSize.height);

    rt->begin();
    this->visit();
    rt->end();
    rt->getSprite()->setAnchorPoint(Vec2(0, 0));

    Scene* ims = InstantMenuScene::create(rt);

    Director::getInstance()->pushScene(ims);
  }
}
```

Тут об'єкт [RenderTexture](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d9/ddc/classcocos2d_1_1_render_texture.html) зберігає зображення, згенероване для старої сцени. [InstantMenuScene](https://github.com/beardog-ukr/cocos2dx-examples/blob/master/examples/AnimationDemo/Classes/scenes/InstantMenuScene.cpp) — це нова сцена, використання переданого зображення там майже не відрізняється від будь-якого іншого, тому я тут його не привожу.
