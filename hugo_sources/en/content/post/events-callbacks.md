---
title: "Events and callbacks"
date: 2020-09-23T17:30:25+03:00
draft: false
tags: ["event", "event processing", "std::bind", "CC_CALLBACK"]
author: beardog-ukr
---

There was one moment in [the default program](/first-steps.md) that still remains unexplained. That's the button that closed the application.
<!--more-->

# Using CC_CALLBACK_1 from MenuItemImage

As you remember the program looked like this:

{{< figure src="/cocos2dx-examples/en/post/first-steps-img/first_app_screenshot.png" title="Default application" position="center" >}}

And after user pressed the button in the bottom left corner the following method was called:
```
void SmokeTestScene::menuCloseCallback(Ref *pSender) {
  // Close the cocos2d-x game scene and quit the application
  Director::getInstance()->end();
}
```

But how does it work? Why the method was called? The whole button gets created like this:
```cpp
/////////////////////////////
// 2. add a menu item with "X" image, which is clicked to quit the program
//    you may modify it.

// add a "close" icon to exit the progress. it's an autorelease object
auto closeItem = MenuItemImage::create(
  "CloseNormal.png",
  "CloseSelected.png",
  CC_CALLBACK_1(SmokeTestScene::menuCloseCallback, this));

if ((closeItem == nullptr) ||
    (closeItem->getContentSize().width <= 0) ||
    (closeItem->getContentSize().height <= 0)) {
  problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
}
else {
  float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
  float y = origin.y + closeItem->getContentSize().height / 2;
  closeItem->setPosition(Vec2(x, y));
}


// create menu, it's an autorelease object
auto menu = Menu::create(closeItem, NULL);
menu->setPosition(Vec2::ZERO);
this->addChild(menu, 1);
```

Most of this code is similar to construction of a joined node we created in [previous post](static-img-practice-app.md). However, there is one more thing: the `CC_CALLBACK_1` macro is used to connect the callback processing method and the menu item.

The `CC_CALLBACK_*` macros id used very often across Cocos2d programs. It's implemented with a bit of C++11 magic [like this](https://github.com/cocos2d/cocos2d-x/blob/v4/cocos/base/ccMacros.h):
```cpp
// new callbacks based on C++11
#define CC_CALLBACK_0(__selector__,__target__, ...) std::bind(&__selector__,__target__, ##__VA_ARGS__)
#define CC_CALLBACK_1(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, ##__VA_ARGS__)
#define CC_CALLBACK_2(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2, ##__VA_ARGS__)
#define CC_CALLBACK_3(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, ##__VA_ARGS__)
```

The `MenuItemImage::create` [was declared](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d1/d30/classcocos2d_1_1_menu_item_image.html#add8af43748de9b1e9d810875ac8fea16) like:
```cpp
static MenuItemImage * 	create (const std::string &normalImage, const std::string &selectedImage, const std::string &disabledImage, const ccMenuCallback &callback)
```
and `ccMenuCallback` here is a [short form](https://github.com/cocos2d/cocos2d-x/blob/v4/cocos/2d/CCMenuItem.h) for `std::function`:
```cpp
typedef std::function<void(Ref*)> ccMenuCallback;
```

So, the `CC_CALBACK_*` macro gets two parameters:
* a function (or method) that should be called
* an object that owns this method.

The number in `CC_CALBACK_*` means the number of arguments the callback function will receive.

As a result you'll get a `std::function` instance created by [std::bind](https://en.cppreference.com/w/cpp/utility/functional/bind). The _Cocos2d-x_ object will call this instance when the actual event will appear.
