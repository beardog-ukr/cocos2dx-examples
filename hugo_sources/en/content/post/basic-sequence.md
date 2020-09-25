---
title: "Sequence"
date: 2020-09-24T15:05:25+03:00
draft: false
tags: ["Sequence", "Action"]
author: beardog-ukr
---

If you need to perform few actions one by one over the same object you can use the [Sequence](https://docs.cocos2d-x.org/cocos2d-x/v4/en/actions/sequences.html) class.

For example the blue ship in our [previous demo]({{< ref "basic-actions.md" >}}) appears to be facing north. If we want it to fly to the right we should first rotate it by 90 degrees and then perform the move.

<!--more-->

In order to do this we create `RotateTo` and `MoveTo` actions and then join them into a sequence:
```cpp
RotateBy* rotateBy = RotateBy::create(2, 90);
MoveBy* moveBy = MoveBy::create(4, Vec2(160,0));

Sequence* seq = Sequence::create(rotateBy, moveBy, nullptr);

blueShip->runAction(seq);
```

The `Sequence`'s constructor method can receive any number of pointers to various actions, but the last pointer should be always `nullptr` (_gcc_ will give you a special kind of warning if it's not). The sequence objects will do all the actions and result will look like this:

{{< figure src="/cocos2dx-examples/en/post/basic-actions-img/blue_ship_move_01.gif" title="move after rotation" position="center" >}}

Also, the _cocos2d-x_ has few special actions that will be useful when building a sequences:

* [DelayTime](https://docs.cocos2d-x.org/api-ref/cplusplus/V3.12/d5/d58/classcocos2d_1_1_delay_time.html) just waits for some amount of time.
* [Repeat](https://docs.cocos2d-x.org/api-ref/cplusplus/V3.12/df/d05/classcocos2d_1_1_repeat.html) and [RepeatForever](https://docs.cocos2d-x.org/api-ref/cplusplus/V3.12/d9/d37/classcocos2d_1_1_repeat_forever.html) repeats given action or sequence.
* [Spawn](https://docs.cocos2d-x.org/api-ref/cplusplus/V3.12/dc/df1/classcocos2d_1_1_spawn.html) launches few actions at the same time.
* [RemoveSelf](https://docs.cocos2d-x.org/api-ref/cplusplus/V3.12/d4/d11/classcocos2d_1_1_remove_self.html#details) removes the object.
* [CallFunc](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d3/d32/classcocos2d_1_1_call_func.html#details) is an action that can call some custom user-defined function. There is an example in [another article]({{< ref "advanced-actions.md" >}})
