---
title: "Animation"
date: 2020-09-25T19:30:25+03:00
draft: false
tags: ["Animation", "AnimationCache", "plist", "TexturePacker", "SpriteFrameCache"]
author: beardog-ukr
---

There are two [kinds of animation](https://hub.packtpub.com/animations-cocos2d-x/):
* frame (spreadsheet) animation: the graphical designers create a series of frames and _Cocos2d_ draws them one by one using [Animation](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d3/dc5/classcocos2d_1_1_animation.html) and [Animate](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d9/db4/classcocos2d_1_1_animate.html) classes. The _Animate_ is a subclass of Action, so it  will be easy to use.
* [Skeletal animation](https://en.wikipedia.org/wiki/Skeletal_animation): a model of the character is created with some special programs and then gets imported to _Cocos2d_. There is [another post]({{< ref "dragonbones-animation.md" >}}) about it.

<!--more-->

# Adding files manually

We'll use a free [candle animation](https://kvsr.itch.io/candle) for  our first example. There are only three frames there so it's easy to load them like this:
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

Here we use `Rect(0,0,64,64)` because the frames are 64х64 pixels size.

The problem is the direct file loading is a bad practice (yet we still used it in our examples because it's easy and intuitive). That's good enough for small educational examples, but real programs should use [spritesheets](https://www.codeandweb.com/what-is-a-sprite-sheet).

# Spritesheets

Here I should recommend you to read this long but very important [article about animation](https://www.codeandweb.com/texturepacker/tutorials/animations-and-spritesheets-in-cocos2d-x). Briefly, there is a [SpriteFrameCache](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d7/d76/classcocos2d_1_1_sprite_frame_cache.html) class and a singleton instance of this class can load the _sprite sheet files_. A _sprite sheet_ is actually a pair of files: one single _*.png_ that combines multiple images and one _*.plist_ which is a XML describing those images.

There are various programs that can be used to create sprite sheets:
* [TexturePacker](https://www.codeandweb.com/texturepacker) is good for starting
* [SpriteSheet Packer](https://github.com/amakaseev/sprite-sheet-packer) is a bit unfinished, but it has a useful "smart folder" feature
* [Shoebox](http://renderhjs.net/shoebox/)
* [Zwoptex](https://zwopple.com/zwoptex/).

For our second example we can take four images of [a creature in the hood](https://opengameart.org/content/little-candle-monsters), create a [plist file](https://github.com/beardog-ukr/cocos2dx-examples/blob/master/examples/AnimationDemo/Resources/littleCandleMonster/littleCandleMonster_violet.plist) and load it into program it like this:
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

You can get rid of listing file names in the code by using the `addAnimationsWithFile` method of the [AnimationCache](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d6/dc7/classcocos2d_1_1_animation_cache.html) instance. Loading will look like this:
```cpp
const string plistFilename = "skeleton/skeleton_images.plist";
SpriteFrameCache::getInstance()->addSpriteFramesWithFile(plistFilename);

const string animationsPlistFN = "skeleton/skeleton_animations.plist";
AnimationCache::getInstance()->addAnimationsWithFile(animationsPlistFN);
```
and the usage of animation like this:
```cpp
Animation* animation = AnimationCache::getInstance()->getAnimation(idleAnimationName);
Animate* animate = Animate::create(animation);
skeletonSprite->runAction(animate);
```

A small problem of this approach is that it requires some additional _plist_ file. This file is different from those created by _TexturePacker_. There is no software to create this kind of files, so you'll have to write it manually. However, that's an usual XML, so it's easy to understand and edit. You can find the file for this example [here](https://github.com/beardog-ukr/cocos2dx-examples/blob/master/examples/AnimationDemo/Resources/skeleton/skeleton_animations.plist).

Anyway, you'll create an _Animate_ instance at the end and this instance can be used as any other _Action_. For example here the violet creature moves like the green UFO from our [previous examples]({{< ref "advanced-actions.md" >}}):

{{< figure src="/cocos2dx-examples/en/post/basic-animations-img/violet_monster.gif" title="Creature moves and changes frames at the same time" position="center" >}}
