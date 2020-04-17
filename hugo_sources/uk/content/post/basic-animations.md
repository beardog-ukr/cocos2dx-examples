---
title: "Анімація"
date: 2020-04-14T17:30:25+03:00
draft: false
tags: ["Animation", "AnimationCache", "plist", "TexturePacker", "SpriteFrameCache"]
author: beardog-ukr
---

Виділяють [два способи](https://hub.packtpub.com/animations-cocos2d-x/) створення анімації:
* покадрова: художники малюють ряд кадрів, а Cocos2d їх послідовно показує, використовуючи клас [Animation](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d3/dc5/classcocos2d_1_1_animation.html). Робота з ним схожа на інші акції, хоча сам клас має трошки інше походження.
* скелетна: у спеціальних програмах формується модель персонажу, а потім завантажується у cocos2dx.

<!--more-->

# Ручний перелік файлів

Для прикладу візьмемо анімацію свічки з [безкоштовного паку](https://kvsr.itch.io/candle). Там всього лише три кадри, їх дуже просто завантажити і використати ось таким чином:
```cpp
Vector<SpriteFrame*> animFrames;
animFrames.reserve(3);
animFrames.pushBack(SpriteFrame::create("candle/SC300-1.png", Rect(0,0,64,64)));
animFrames.pushBack(SpriteFrame::create("candle/SC300-2.png", Rect(0,0,64,64)));
animFrames.pushBack(SpriteFrame::create("candle/SC300-3.png", Rect(0,0,64,64)));

// create the animation out of the frames
Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.4f);
Animate* animate = Animate::create(animation);

// run it and repeat it forever
Sprite* candleSprite = Sprite::create();
candleSprite->runAction(RepeatForever::create(animate));
```

Тут цифри у `Rect(0,0,64,64)` такі, тому що файли розміром 64х64 пікселі.

Проблема в тому, що практика прямого завантаження файлів, яку ми досі використовували у всіх прикладах, насправді є досить поганою. Вона підходить для навчання, але краще буде використовувати [списки файлів](https://www.codeandweb.com/what-is-a-sprite-sheet) (_sprite sheet_).

# Файли зі списком файлів

У цьому місці я б дуже радив прочитати ось цю довгу, але дуже корисну [статтю про анімацію](https://www.codeandweb.com/texturepacker/tutorials/animations-and-spritesheets-in-cocos2d-x). Якщо коротко, то є такий клас [SpriteFrameCache](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d7/d76/classcocos2d_1_1_sprite_frame_cache.html) може завантажувати зображення, описані у файлах _*.plist_. Такі файли можна створювати у різних програмах: непоганим варіантом є [TexturePacker](https://www.codeandweb.com/texturepacker), також можна використовувати [Shoebox](http://renderhjs.net/shoebox/), [SpriteSheet Packer](https://github.com/amakaseev/sprite-sheet-packer) та [Zwoptex](https://zwopple.com/zwoptex/). Результатом пакування є два файли:
* одне велике зображення, що об'єднує усі передані ресурси. Завдяки тому, що це один файл, він займає менше місця та швидше завантажується
* файл _*.plist_, який описує упаковані ресурси.

Наприклад, можна взяти чотири зображення [створіння у капюшоні](https://opengameart.org/content/little-candle-monsters), створити на його основі plist та ресурс і використати ось так:
```cpp
SpriteFrameCache* sfc = SpriteFrameCache::getInstance();

const string violetMonsterFN = "littleCandleMonster/littleCandleMonster_violet.plist";
sfc->addSpriteFramesWithFile(violetMonsterFN);

Vector<SpriteFrame*> animFrames;
Animation *monsterAnimation = Animation::create();
char tmps[256];
for (int i = 1; i<=4; i++) {
  sprintf(tmps, "lm-1-%i.png", i);
  SpriteFrame * sf = sfc->getSpriteFrameByName(tmps);
  monsterAnimation->addSpriteFrame(sf);
}

monsterAnimation->setDelayPerUnit(0.1);

Animate* animate = Animate::create(monsterAnimation);
```

Можна позбавитись від переліку імен файлів у коді, якщо використати метод `addAnimationsWithFile` класу [AnimationCache](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d6/dc7/classcocos2d_1_1_animation_cache.html). Тоді завантаження анімацій виглядатиме так:
```cpp
const string plistFilename = "skeleton/skeleton_images.plist";
SpriteFrameCache::getInstance()->addSpriteFramesWithFile(plistFilename);

const string animationsPlistFN = "skeleton/skeleton_animations.plist";
AnimationCache::getInstance()->addAnimationsWithFile(animationsPlistFN);
```
А використання так:
```cpp
Animation* animation = AnimationCache::getInstance()->getAnimation(idleAnimationName);
Animate* animate = Animate::create(animation);
skeletonSprite->runAction(animate);
```
Недоліком такого способу є те, що він вимагає якогось особливого plist-файлу. Цей файл відрізняється від тих, що створює _Texture Packer_ (не зважаючи на однакове розширення) і його доводиться виписувати вручну. Для даного прикладу цей файл виглядатиме так.

У будь-якому разі, отримана в результаті акція `animate` може використовуватись так само, як і будь-яка інша акція. Ось, наприклад, фіолетове створіння пересувається так само, як і зелене НЛО з [попередньої статті]({{< ref "advanced-actions.md" >}})

{{< figure src="/cocos2dx-examples/uk/post/basic-animations-img/violet_monster.gif" title="" position="center" >}}
