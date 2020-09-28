---
title: "Scrolling the map"
date: 2020-09-28T02:55:25+03:00
draft: false
tags: ["design resolution", "scrolling"]
---


Sometimes in your game you'll have to scroll the map when your character moves to some new position. For some games the scrolling creates a really significant part of user experience; I'd recommend to read [this article](https://www.gamasutra.com/blogs/ItayKeren/20150511/243083/Scroll_Back_The_Theory_and_Practice_of_Cameras_in_SideScrollers.php) just to understand how important it is.

<!--more-->

For your example we'll take an [evil mage](https://luizmelo.itch.io/evil-wizard) animation. Our mage will walk through a long long level that looks like some [castle](https://szadiart.itch.io/rogue-fantasy-castle).

{{< figure src="/cocos2dx-examples/en/post/scrolling-img/dungeon02.png" title="The level map is few times wider than the screen width" position="center" >}}

The simplest way to implement map scrolling in _cocos2d-x_ is by using the [Camera](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d6/d2b/classcocos2d_1_1_camera.html) class.

[Camera](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d6/d2b/classcocos2d_1_1_camera.html) is another [Node](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d3/d82/classcocos2d_1_1_node.html) subclass, so we can use some usual  [MoveTo](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/de/d42/classcocos2d_1_1_move_to.html) action to move it. However, unlike most of the other objects, the camera lives in 3D, so coordinates are specified a bit differently.


That's how we move the camera to the mage's position:
```cpp
cocos2d::Vec2 expectedMagePos; // mage position
<....>
Camera* camera = getDefaultCamera();
const Vec3 currentCameraPos = camera->getPosition3D();
Vec3 newCameraPos = Vec3(expectedMagePos.x, expectedMagePos.y, currentCameraPos.z);
MoveTo* cameraMoveAction = MoveTo::create(1, newCameraPos);
camera->runAction(cameraMoveAction);
```
Here we create a new position point for the camera; it's same as the mage position, but also it keeps Z position of the camera. 

And that's all, nothing more. You can see how it works in the [demo program](https://github.com/beardog-ukr/cocos2dx-examples/blob/master/examples/TiledBackground/Classes/scenes/HorizontalScrollScene.cpp). The camera moves each time the mage goes close enough to the right wall:

{{< figure src="/cocos2dx-examples/en/post/scrolling-img/mage_walks.gif" title="Mage moves right" position="center" >}}

You could also notice the sceletons here, and there are two important things to say about them. First, some of them move out of the screen borders, so there probably is some waste of CPU resources for calculating their position. You could think it will be better to somehow turn off the animations for all the objects that are too far from the main game character. But this idea is an [early optimization](https://gamedev.stackexchange.com/questions/141399/why-is-it-so-bad-to-optimize-too-early), and it's better to [avoid it](https://love2d.org/wiki/Optimising).

Second, since there are few skeleton persons on the map it will be better to create a special class for a skeleton. Actually, the mage also could use its own class, but to simplify the code I've stored all related data in the main class of [the scene](https://github.com/beardog-ukr/cocos2dx-examples/blob/master/examples/TiledBackground/Classes/scenes/HorizontalScrollScene.h).

There are two possible ways for creating an object's class in _cocos2d-x_:
* you can subclass your class from [cocos2d::Sprite](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d3/d5c/classcocos2d_1_1_sprite.html) (or from [Node](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d3/d82/classcocos2d_1_1_node.html) if it consists from multiple sprites)
* create a class that will manage the [cocos2d::Sprite](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d3/d5c/classcocos2d_1_1_sprite.html) instance

Both solutions are [quite good](https://discuss.cocos2d-x.org/t/cocos2d-x-general-question-about-class-design-and-inheritance/24893). Sometimes it comes to more common question "[composition over inheritance](https://en.wikipedia.org/wiki/Composition_over_inheritance)" and all possible choices have some bad and good sides. Here in the example I've used [inheritance](https://github.com/beardog-ukr/cocos2dx-examples/blob/master/examples/TiledBackground/Classes/scenes/SkeletonPerson.h), but if you prefer composition you can find some useful advices in [this article](http://becomingindiedev.blogspot.com/2015/06/correct-composition-design-in-cocos2d-x.html).
