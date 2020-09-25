---
title: "Advanced usage of CC_CALLBACK"
date: 2020-09-25T16:30:25+03:00
draft: false
tags: ["callbacks", "events", "CC_CALLBACK"]
author: beardog-ukr
---

In the previous [article about callbacks]({{< ref "events-callbacks.md" >}}) I've said that the number in the `CC_CALLBACK_*` macro means the amount of parameters need for called function. That wasn't exactly true because the C++ magic allows to do a one more trick here.

<!--more-->

Let's look again the green UFO from [this article]({{< ref "advanced-actions.md" >}}). The only difference between moving to the right and moving to the left is the coordinate of destination point. Obviously, it would be better to process both movements with the same method; if only we could call this method with different parameters.

The trick is that the `CC_CALLBACK_` macro can receive any amount of parameters. The [std::bind](https://en.cppreference.com/w/cpp/utility/functional/bind) will transfer all the parameters to the called function.

Here is the example. The common callback for the button pressing is declared like this:
```cpp
class ActionsDemoScene : public cocos2d::Scene {
public:
  // .....
protected:
  void ufoMoveCallback(cocos2d::Ref *pSender, const int direction);
};
```

Here `direction` is the additional parameter we would like to transfer to called function.

And here is the `CC_CALLBACK_1` usage:
```cpp
MenuItemImage* leftButton = MenuItemImage::create(
    "ui/leftArrow1.png", "ui/leftArrow2.png",
    CC_CALLBACK_1(ActionsDemoScene::ufoMoveCallback, this, GU_MOVE_LEFT));

MenuItemImage* rightButton = MenuItemImage::create(
  "ui/rightArrow1.png", "ui/rightArrow2.png",
  CC_CALLBACK_1(ActionsDemoScene::ufoMoveCallback, this, GU_MOVE_RIGHT));
```
Here `GU_MOVE_LEFT` and `GU_MOVE_RIGHT` are the constant values to differ between movement directions.

So, the common callback will be implemented like:
```cpp
void ActionsDemoScene::ufoMoveCallback(Ref *pSender, const int direction) {
  float newX = 0; //undefided at this point
  if (direction == GU_MOVE_RIGHT) {
    newX = rightX;
  }
  else {
    newX = leftX;
  }

  // use the newX value
}
```

Also, we could do one more step and provide the new coordinate value right to `CC_CALLBACK_1` instead of the special constant values. Then we could get rid of `if` in the method implementation. However, I thought it would make the code less readable.
