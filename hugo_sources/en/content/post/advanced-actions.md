---
title: "Advanced actions"
date: 2020-09-25T10:55:25+03:00
draft: false
tags: ["Action", "tag", "Sequence", "CallFunc"]
---

The [previous article]({{< ref "basic-actions.md" >}}) described how to use [Action](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/db/d61/classcocos2d_1_1_action.html) to move the objects. Now we'll learn how to do more complicated stuff.

<!--more-->

# The task

But first let's create a task to implement.

We have to create a scene with two elements: a green UFO and a blue ship.

1. The UFO should constantly rotate around it's center slightly
1. There should be two buttons: left and right arrows. After pressing the button the UFO should start moving in appropriate direction until it reaches the "finish" position
1. If the user presses another button during movement, the ship stops and then starts to move to another side.
1. Blue ship starts on the left side of the screen, facing up (north). There is a button (arrow) on the right side of the screen
1. After the button is pressed the sheep rotates and then moves towards right.
1. When the ship reaches destination it turns nose up again. The arrow button changes direction.
1. If the user presses the arrow button again the ship rotates and moves to another side.

So, the green UFO will work like this:
{{< figure src="/cocos2dx-examples/en/post/advanced-actions-img/ufo_move_sides.gif" title="The green UFO moves from side to side and rotates slightly" position="center" >}}

And the blue ship:
{{< figure src="/cocos2dx-examples/en/post/advanced-actions-img/blue_ship_move.gif" title="Blue ship behavior" position="center" >}}

I should remind you the images here came from free image sets: [buttons](https://evolutionarygames.itch.io/interface-elements-mobile-metallic) and [spaceships](https://www.kenney.nl/assets/space-shooter-redux)

# Action tags

The [Action](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/db/d61/classcocos2d_1_1_action.html) class has a special "_tag_" parameter. That's an integer value used to distinguish one action from another. You can set this value with _setTag_ method or sometimes provide as a parameter to constructor.

Also, the [Node](https://docs.cocos2d-x.org/api-ref/cplusplus/V3.12/d3/d82/classcocos2d_1_1_node.html) class has some methods for processing action tags:
* `void stopActionByTag(int tag)` stops action with given tag (if there is any)
* `Action* getActionByTag(int tag)` returns a pointer to a running action with this tag. Note there may be few actions with the same tag running at the same time; I guess the behavior for this case is undefined.
* `ssize_t getNumberOfRunningActionsByTag(int tag) const` returns amount of running actions with given tag

And there is a singleton instance of [ActionManager](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d1/d88/classcocos2d_1_1_action_manager.html#details) with similar methods, but it's not recommended using it.

In our example we'll use tags to distinguish UFO rotation and horizontal movement. As for rotation, it's implemented with the corresponding action and the [RepeatForever](https://docs.cocos2d-x.org/api-ref/cplusplus/V3.12/d9/d37/classcocos2d_1_1_repeat_forever.html):
```cpp
Sequence* rseq = Sequence::create(RotateBy::create(3, 30),
                                  RotateBy::create(3, -30), nullptr);

RepeatForever* reps = RepeatForever::create(rseq);
reps->setTag(AT_UFO_ROTATION);
greenUfo->runAction(reps);
```
Here `AT_UFO_ROTATION` is an integer constant used as tag value.

The UFO launch looks like this:
```cpp
MoveBy* moveTo = MoveTo::create(time, Vec2(newX,240));
moveTo->setTag(AT_UFO_MOVING);
greenUfo->runAction(moveTo);
```
and stopping like this
```cpp
greenUfo->stopAllActionsByTag(AT_UFO_MOVING);
```
Here `stopAllActionsByTag` stops the action marked by `AT_UFO_MOVING` tag, but the rotation continues.

In case of blue ship we'll have to stop the whole sequence. There is not much difference from single action; just set tag to the sequence and not on the action.

```cpp
// create some actions
///....

Sequence* seq = Sequence::create(rotateAct, moveAct, rotateBackAct, nullptr);
seq->setTag(AT_BS_MOVE);

blueShip->runAction(seq);
```

# Receiving an action

According to the task, it's not allowed to interrupt the blue ship's rotation. In order to do this we'll have to check if the action is running at the moment.

There are two methods we can use for it : `getNumberOfRunningActionsByTag()` and `getActionByTag()`.

The code will be almost same for both of them:
```cpp
if (blueShip->getNumberOfRunningActionsByTag(AT_BS_ROTATION_BACK)>0 ) {
  log("%s: rotating back, do not interfere with moving", __func__);
  return;
}
```
or
```cpp
if (blueShip->getActionByTag(AT_BS_ROTATION_BACK) != nullptr) {
  log("%s: rotating back, do not interfere", __func__);
  return;
}
```

# Processing action finish

The task says the image on arrow button must be performed right after the ship reaches its destination. We can do it with an instance of  [CallFunc](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d3/d32/classcocos2d_1_1_call_func.html#details) class. This action calls the function provided to the constructor.

So, we can create a [lambda-expression](https://uk.wikipedia.org/wiki/%D0%9B%D1%8F%D0%BC%D0%B1%D0%B4%D0%B0-%D0%B2%D0%B8%D1%80%D0%B0%D0%B7%D0%B8_%D1%83_%D0%A1%2B%2B), and provide it to the _CallFunc_'s constructor:
```cpp
CallFunc* finalCf = CallFunc::create([this]() {
  // create and launch action to rotate the ship back
  RotateTo* ra2 = RotateTo::create(3, 0);
  ra2->setTag(AT_BS_ROTATION_BACK);
  this->blueShip->runAction(ra2);

  // change image on the button
  // ......
});

Sequence* seq = Sequence::create(rotateAct, moveAct, finalCf, nullptr);
seq->setTag(AT_BS_MOVE);

blueShip->runAction(seq);
```

As you already know, the `Sequence` constructor receives all the actions and as parameters and the `nullptr` at the end. Here we perform three actions: rotation, movement and callback. Then callback will add one more action to rotate the ship back.
