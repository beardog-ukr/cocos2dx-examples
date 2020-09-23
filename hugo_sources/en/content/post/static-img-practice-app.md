---
title: "The Sprite class"
date: 2020-09-23T02:30:25+03:00
draft: false
tags: ["Sprite", "Static Images"]
author: beardog-ukr
---

In this post we'll try to create our really own program. It will not do much, only show a few spaceships images from a [free pack](https://www.kenney.nl/assets/space-shooter-redux). Our goal is to learn about [Sprite](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d3/d5c/classcocos2d_1_1_sprite.html) class and its common properties.
<!--more-->

# Coordinates and the anchor point

Cocos2d uses "classic" coordinate system:
* x grows horizontally from left to right
* y grows vertically from down to up.
* origin (0;0) appears at bottom left corner ([here](https://gamefromscratch.com/cocos2d-x-tutorial-series-basic-sprites-positioning-parenting-and-coordinate-systems/) you can read more about it)

Also there is an "anchor point". It's a base point for every transformation that's performed on a sprite. You can set it with [setAnchorPoint](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d3/d82/classcocos2d_1_1_node.html#abf77210f31f01d0f564600c94c90e2be) method which accepts two values from 0 to 1. Usually this values are 0, 0.5 or 1:
* setActionPoint(Vec2(0.5, 0.5)) sets anchor point at the center of the sprite.
* setActionPoint(Vec2(0, 0)) sets anchor to bottom left.
* setActionPoint(Vec2(0, 1)) sets anchor to top left corner.

When you use `setPosition` method it actually positions the anchor point and the rest of the sprite gets aligned around it.

# A program with background

To see how this all works we'll use an image that was made of gray rectangles. Each rectangle is  160x120 size.

{{< figure src="/cocos2dx-examples/en/post/static-img-practice-app/empty_grid.png" title="Empty grid background, axis, sizes" position="center" >}}

Following code creates this thing:
```
const char backFilename[] = "background.png";

Sprite* sprite = Sprite::create(backFilename);
if (sprite == nullptr) {
  printf("Error while loading: %s\n", backFilename);
  return false;
}

sprite->setAnchorPoint(Vec2(0,0));
sprite->setPosition(0,0);
addChild(sprite, 0);
```

Here we create the sprite object with a static [create](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d3/d5c/classcocos2d_1_1_sprite.html#a3f75a553fe02f3de82042e81b76da010) method. That's a common practice for _cocos2d-x_; every class has a method like this and it should be used to create new objects instead of traditional `new`.

Then we set the sprite's anchor point at bottom left corner and position the sprite at (0;0) which is a beginning of the scene's coordinate system.

Finally, we use `addChild` to add the sprite to the scene. The second parameter for this method is a _z-order_: this value defines priority during drawing. Elements with larger number will be drawn later, so they'll appear over those ones with lower number.

# More sprites

Now let's add some more sprites. One will use following code:
```cpp
const char shipFilename[] = "ships/playerShip3_blue.png";

Sprite* sprite = Sprite::create(shipFilename);

sprite->setAnchorPoint(Vec2(0.5,0.5));
sprite->setPosition(Vec2(160, 240));

addChild(sprite, 10);
```
And another with this code:
```cpp
const char shipFilename[] = "ships/playerShip3_blue.png";
Sprite* sprite = Sprite::create(shipFilename);

sprite->setAnchorPoint(Vec2(0.5,0.5));
sprite->setRotation(45);
sprite->setPosition(Vec2(160, 120));

addChild(sprite, 10);
```

The difference here is that second sprite gets rotated by 45 degrees.

Both sprites have anchor point at (0.5,0.5), so the center of the sprites should appear between the squares on the screen:

{{< figure src="/cocos2dx-examples/en/post/static-img-practice-app/normal.png" title="Blue ships, anchor point at the center" position="center" >}}

Now let's add some more sprites:
```cpp
const char shipFilename[] = "ships/playerShip3_green.png";
Sprite* sprite = Sprite::create(shipFilename);

sprite->setAnchorPoint(Vec2(0,0));
sprite->setPosition(Vec2(480, 240));

addChild(sprite, 10);

```
and:
```cpp
const char shipFilename[] = "ships/playerShip3_green.png";
Sprite* sprite = Sprite::create(shipFilename);

sprite->setAnchorPoint(Vec2(0,0));
sprite->setRotation(45);
sprite->setPosition(Vec2(480, 120));

addChild(sprite, 10);
```

That's the same as previous pair, but the anchor point is at bottom left corner of the sprite. Again, second sprite gets rotated by 45 degrees.

Result looks like

{{< figure src="/cocos2dx-examples/en/post/static-img-practice-app/cornered.png" title="Green ships, anchor point at bottom left" position="center" >}}

Here the anchor of the sprite is positioned at the crossing, but the sprite itself becomes upper and more to the right. Also, in second case the rotation is performed against the anchor point.

# Node

The _Node_ object can be used to group few other object together. For example, we can build our own spaceship by adding three images:

{{< figure src="/cocos2dx-examples/en/post/static-img-practice-app/mixproj.png" title="Building a ship from parts" position="center" >}}

This can be done like:
```cpp
const char cockpitFilename[] = "parts/cockpitRed.png";
Sprite* cockpitSprite = Sprite::create(cockpitFilename);

const char wingFilename[] = "parts/wingRed.png";
Sprite* rightWingSprite = Sprite::create(wingFilename);
Sprite* leftWingSprite = Sprite::create(wingFilename);

Node* resultNode = Node::create();
resultNode->setAnchorPoint(Vec2(0.5,0.5));

//
rightWingSprite->setAnchorPoint(Vec2(0.5,0.5));
rightWingSprite->setScale(1, -1);
rightWingSprite->setPosition(Vec2(60, 15));
rightWingSprite->setRotation(90);
resultNode->addChild(rightWingSprite);

leftWingSprite->setAnchorPoint(Vec2(0.5,0.5));
leftWingSprite->setPosition(Vec2(-60, 15));
leftWingSprite->setRotation(90);
resultNode->addChild(leftWingSprite);

resultNode->addChild(cockpitSprite);
//
resultNode->setAnchorPoint(Vec2(0.5,0.5));
```

First, we create three sprites (body and two wings) and an empty _Node_ object that will join all the parts together.

We rotate the wing sprites and put them at correct position. Coordinates (60, 15) were selected empirically, just to get something that looks more or less good. Also the right wing sprite should be mirrored, the `setScale(1, -1)` function does that.

Finally, all the sprites became the children of the node object. Now this node object can be processed (added to scene, rotated) as any other sprite, so I will not show this code here. You can find the code for the whole project on [github](https://github.com/beardog-ukr/cocos2dx-examples/blob/master/examples/StaticSprites/Classes/StaticSpritesScene.cpp).

So our resulting program will look like this:

{{< figure src="/cocos2dx-examples/en/post/static-img-practice-app/mixed.png" title="Red ships are a combination of nodes, but you can work with them as with single node." position="center" >}}
