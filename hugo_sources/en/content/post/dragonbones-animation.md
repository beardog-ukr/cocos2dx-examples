---
title: "Animation with Dragon Bones"
date: 2020-09-26T07:30:25+03:00
draft: false
tags: ["Animation", "Dragon Bones", "third-party tools", "skeletal animation"]
author: beardog-ukr
---

There are [many variants](https://www.slant.co/options/15725/alternatives/~dragonbones-pro-alternatives) to create a [skeletal animation](https://en.wikipedia.org/wiki/Skeletal_animation) but almost all of them are paid and so not available for educational purposes. In this article we'll work with [Dragon Bones](https://dragonbones.github.io/en/index.html), that's a free, but a bit weird tool.

The _Dragon Bones_ has decent functionality and it's free. To be presise the editor itself is free as in beer (closed sources) but the code for integrating models into _cocos2d-x_ is distributed under MIT license.

<!--more-->

On the bad side, it's a bit unstable. For example, few months ago it was hard to download the editor from [official page](https://dragonbones.github.io/en/download.html) because it got some issues with modern browsers. There is a code for integration DB files into _cocos2d-x_ in the [official repository](https://github.com/DragonBones/DragonBonesCPP/tree/master/Cocos2DX_3.x), but it's obsolete and will cause some compilation errors. I'd recommend to use [this](https://github.com/beardog-ukr/dragonbones-cocos2dx-v4-demo) repository because it has all tweaks required for Cocos2d V4.

Anyway, after getting it up and running the _Dragon Bones_ editor starts to work seamlessly and allows you to create a really cool things like this one:

{{< figure src="/cocos2dx-examples/en/post/dragon-bones-img/dragon_bones_mecha.gif" title="Example CoreElementGame from official repo" position="center" >}}

Just note, this mecha is really a 2d picture, you can see it [here](https://github.com/beardog-ukr/dragonbones-cocos2dx-v4-demo/blob/master/DemoProj/Resources/mecha_1502b/mecha_1502b_tex.png) stripped into parts.

For our example we'll take medieval persons from [this free pack](https://jordan97.itch.io/cartoon-character-kit) and will give them some [swords](https://krukowski.itch.io/free-weapons) and [guns](https://kaylousberg.itch.io/gun-assets).

Here I'm not going to tell how to create the models and animations. It's quite complicated and really that's more for graphical designers and not for software developers. There is a good [tutorial here](http://getting-started-dragonbones.blogspot.com/2017/01/dragonbones-tutorial-part-i.html), and [here](https://habr.com/ru/post/350254/), and [here](https://topic.alibabacloud.com/a/a-tutorial-on-using-the-dragonbones-of-the-skeleton-animation-tool_8_8_31057239.html). 

Anyway, after you create a model and export it from _Dragon Bones_ to _Cocod2d_ it will give you [three files](https://github.com/beardog-ukr/cocos2dx-examples/tree/master/examples/DragonBonesImport/Resources/db_export): two JSONs defining the model and one image with all resources.

Import into the program is similar to opening _plist_ files:
```cpp
#include "dragonBones/cocos2dx/CCDragonBonesHeaders.h"

dragonBones::CCFactory* factory = dragonBones::CCFactory::getFactory();
factory->loadDragonBonesData("db_export/BlueWizard_ske.json", "BlueWizard");
factory->loadTextureAtlasData("db_export/BlueWizard_tex.json", "BlueWizard");

dragonBones::CCArmatureDisplay* wizard = factory->buildArmatureDisplay("WizardArmature", "BlueWizard");

wizard->setAnchorPoint(Vec2(0.5,0.5));
wizard->setScale(0.33);
wizard->setPosition(160,140);
addChild(wizard, 10);
```
[CCArmatureDisplay](https://github.com/beardog-ukr/dragonbones-cocos2dx-v4-demo/blob/master/DemoProj/Classes/dragonBones/cocos2dx/CCArmatureDisplay.h) is the class for objects you can see on the scene. It's a subclass of _Node_, so you can do with it anything you used to do with other scene elements.

"BlueWizard" here is a kind of tag that allows to distinguish this loaded model from all others. The documentation references this parameter as _displayName_.

"WizardArmature" is an armature name (_armatureName_), it was specified in the editor like this:

{{< figure src="/cocos2dx-examples/en/post/dragon-bones-img/db_screen_01a.png" title="" position="center" >}}

Now if you want to play the animation:
```cpp
wizard->getAnimation()->play("idle01", 1);//1 to play animation once
```
That's how it looks:
{{< figure src="/cocos2dx-examples/en/post/dragon-bones-img/wizard_move.gif" title="" position="center" >}}

It's possible to change a part of armature in the program and there are two ways to do it.

First, the editor allows storing few images in one slot. For example, here there are three images in the "Gun" slot:
{{< figure src="/cocos2dx-examples/en/post/dragon-bones-img/db_screen_02.png" title="" position="center" >}}

In this case you can switch to another image simply by specifying index:
```cpp
int currentWeaponIndex;
<....>
wizard->getArmature()->getSlot("Gun")->setDisplayIndex(currentWeaponIndex);
```

Second, you can create a [special model](https://github.com/beardog-ukr/cocos2dx-examples/tree/master/examples/DragonBonesProjects/WeaponPack) that will contain only images to be changed. This makes loading and usage more complicated:
```cpp
dragonBones::CCFactory* factory = dragonBones::CCFactory::getFactory();
factory->loadDragonBonesData("db_export/MagentaKnight_ske.json", "MagentaKnight");
factory->loadTextureAtlasData("db_export/MagentaKnight_tex.json", "MagentaKnight");

factory->loadDragonBonesData("db_export/WeaponPack_ske.json", "WeaponPackMK");
factory->loadTextureAtlasData("db_export/WeaponPack_tex.json", "WeaponPackMK");

knight = factory->buildArmatureDisplay("KnightArmature", "MagentaKnight");

<..later..>
factory->replaceSlotDisplay("WeaponPackMK", "WeaponPackArmature", "OneHandWeaponSlot",
                            "morning_star", knight->getArmature()->getSlot("Weapon"));
```
Here "WeaponPackArmature" and "OneHandWeaponSlot" are the names of armature and slot specified in the model:
{{< figure src="/cocos2dx-examples/en/post/dragon-bones-img/db_screen_03.png" title="" position="center" >}}

In both cases result will be the same:
{{< figure src="/cocos2dx-examples/en/post/dragon-bones-img/weapon_change.gif" title="" position="center" >}}
