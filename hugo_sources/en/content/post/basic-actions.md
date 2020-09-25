---
title: "Movement and other object transformations"
date: 2020-09-24T10:55:25+03:00
draft: false
tags: ["Action", "MoveTo"]
---

In previous articles we learned how to show images and process keyboard event. Now it's time to do something interesting with objects on the screen.

<!--more-->

Imagine you want to move some sprite from one place on the screen to another. You can't just use `setPosition` because it will look like teleportation. Instead, you should set new object position a bit closer to the target, then after a very small period of time (milliseconds, maybe) change position  again. And again, and again, until after a thousand of small moves the object will appear  where you want to see it. That's not an easy task to implement this in code, so _cocos2d-x_ developers created a special class for it, called [Action](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/db/d61/classcocos2d_1_1_action.html).

For example, in order to move something you should use a `MoveTo` class, which is a subclass of `Action`. It looks like this:
```cpp
void ActionsDemoScene::sideButtonCallback(Ref *pSender) {
  MoveTo* moveTo = MoveTo::create(2, Vec2(320,240));
  greenUfo->runAction(moveTo);
}
```
Here _greenUfo_ is a sprite on the scene. `moveBy` is an action that moves the sprite. The [factory method](https://en.wikipedia.org/wiki/Factory_method_pattern) `MoveTo::create` receives two parameters:
* time (in seconds) during which the transformation should be completed
* _Vec2_ a two numbers array that defines a destination point.

As a result, after pressing a button the green ship will start moving and after some amount of time will get to another place.

{{< figure src="/cocos2dx-examples/en/post/basic-actions-img/ufo_move_01.gif" title="Move ship from one point to another" position="center" >}}

# Transformations

There are a [lot of](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/dd/d0d/group__actions.html) possible transformations in _cocos2d-x_:
* [MoveTo](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/de/d42/classcocos2d_1_1_move_to.html) and [MoveBy](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d6/d7c/classcocos2d_1_1_move_by.html) move the sprites
* [RotateTo](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d0/d71/classcocos2d_1_1_rotate_to.html) rotate [RotateBy](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d0/d28/classcocos2d_1_1_rotate_by.html) objects by some degree
* [FadeTo](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d9/de6/classcocos2d_1_1_fade_to.html), [FadeIn](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d0/d0a/classcocos2d_1_1_fade_in.html) and [FadeOut](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/dd/d49/classcocos2d_1_1_fade_out.html) change opacity
* [SkewTo](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d7/d39/classcocos2d_1_1_skew_to.html) and [SkewBy](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d2/d6c/classcocos2d_1_1_skew_by.html) performs something hard to describe, it's better to [see it](https://www.youtube.com/watch?v=X7I-oQYxnnE)

Most of the transformations come in two variants: "To" and "By". The first one receives new parameters (angle, point etc) exactly, and the latter gets a difference between current and desired values.

# Easing

By default, the _Action_ changes parameters smoothly and equally, but it may be changed. For example the value may be changed slowly at first and more significantly later; this will give an impression of moving a massive body.

Custom behavior may be implemented with [Easing*](https://docs.cocos2d-x.org/cocos2d-x/v4/en/actions/basic.html#easing) classes. There are a lot of different variants of them in _cocos2d-x_.

A small example:
```cpp
MoveBy* bma = MoveBy::create(6, Vec2(360,0));
EaseIn* bea = EaseIn::create(bma, 1.5f);
blueShip->runAction(bea);
```
Second parameter for _EaseIn_ constructor defines how notably the changes will be. The larger the easier it will be to see the difference in motion.

Following picture shows the [EaseIn](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/dd/dde/classcocos2d_1_1_ease_in.html) and [EaseOut](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/da/d63/classcocos2d_1_1_ease_out.html) comparing to default movement:

Результат модифікаторів [:
{{< figure src="/cocos2dx-examples/en/post/basic-actions-img/easing_triple.gif" title="Easing" position="center" >}}

Here the green ship moves as default, blue one gets _EaseIn_ and the red one was eased out.
