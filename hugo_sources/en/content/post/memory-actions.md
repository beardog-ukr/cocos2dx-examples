---
title: "How to remove objects"
date: 2020-09-25T18:30:25+03:00
draft: false
tags: ["child-parent", "CallFunc", "RemoveSelf", "memory management"]
author: beardog-ukr
---

Since we already know about [actions]({{< ref "basic-actions.md" >}}) and other [basic things]({{< ref "basic-actions.md" >}}), now we can think about correct memory usage.

To illustrate it we'll create a [simple scene](https://github.com/beardog-ukr/cocos2dx-examples/blob/master/examples/AdvancedActions/Classes/MemoryDemoScene.cpp) with one sprite and one button. After pressing the button the sprite will out of the scene and then it should be removed somehow.

<!--more-->

Actually, there will be few variants of object removal, so we'll make a menu instead of one button:

{{< figure src="/cocos2dx-examples/en/post/advanced-actions-img/memory_remove.gif" title="Simple illustration scene" position="center" >}}

Our variants are
* do nothing and leave the object as is (that's bad, but at least there will be no segfaults).
* use [RemoveSelf](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d4/d11/classcocos2d_1_1_remove_self.html) which is the best practice
* use [Node](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d3/d82/classcocos2d_1_1_node.html)'s `removeChild()` is a bit complicated and can be [dangerous](https://discuss.cocos2d-x.org/t/why-removefromparent-removechild-could-be-dangerous/32223)
* keep the object and use it for a second time.

# A class for debugging

In order to ensure that object gets deleted (or not deleted) we'll create  a special subclass of [Sprite](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d3/d5c/classcocos2d_1_1_sprite.html). It will print debugging messages in constructor and destructor.

Something like this:
```cpp
class EnhancedSprite : public cocos2d::Sprite {
public:
  static EnhancedSprite* create(const std::string& filename);
protected:
  EnhancedSprite();
  virtual ~EnhancedSprite();

  int objectId;
};

static int objectIdGenerator = 0;

EnhancedSprite::EnhancedSprite() {
  objectId = objectIdGenerator++;
  log("EnchancedSprite constructor here, id %02d", objectId);
}

EnhancedSprite::~EnhancedSprite() {
  log("EnchancedSprite destructor here, id %02d", objectId);
}

EnhancedSprite* EnhancedSprite::create(const std::string& filename) {
  EnhancedSprite *result = new (std::nothrow) EnhancedSprite();
  if (result && result->initWithFile(filename)) {
    result->autorelease();
    return result;
  }
  CC_SAFE_DELETE(result);
  return nullptr;
}
```

Here each object an `objectId` field with its own unique value. We can use this value to distinguish one object from another. Everything else remains the same as default _Sprite_.

# Variants

If you do nothing the objects will remain in memory. However, if you'll try to close the application you'll see the destructors called in the end. That's because the sprites are the children of the scene; when the scene finally gets deleted it will delete all the children. So, strictly speaking this is not a memory loss, but still it's a bad situation and everybody should avoid it.

The best and most simple way to remove the sprite is to add [RemoveSelf](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d4/d11/classcocos2d_1_1_remove_self.html) to the end of sequence like this:
```cpp
MoveTo* moveAct = MoveTo::create(5, Vec2(800, 120));
Sequence* seq = Sequence::create(moveAct, RemoveSelf::create(), nullptr);

blueShip->runAction(seq);
```

This code will move the sprite and later you'll see that destructor was called.

Also you can use a [CallFunc](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d3/d32/classcocos2d_1_1_call_func.html) and `removeChild()`:
```cpp
MoveTo* moveAct = MoveTo::create(5, Vec2(800, 120));

Sprite* oldShip = blueShip; //save blueShip pointer because it may get changed by the time
                            // the ship reaches "out of range: destination
CallFunc* finalCf = CallFunc::create([this, oldShip]() {
  log("%s: callback implemented in processMenuRemoveChild: move finished", __func__);
  this->removeChild(oldShip);
});

Sequence* seq = Sequence::create(moveAct, finalCf, nullptr);

blueShip->runAction(seq);
```
This second approach is a bit complicated but still good.

# Keeping and reusing the object

Maybe you want to keep the object and save it into a [pool](https://en.wikipedia.org/wiki/Pool_(computer_science)). That's possible, but requires additional explanations.

Almost all classes in _cocos2d-x_ are subclassed from the [Ref](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/df/d28/classcocos2d_1_1_ref.html) class. The framework developers wanted to make something similar to Objective-C memory management. So, each object has following methods:
* `retain()` increases reference counter, "protects" the object from removal
* `release()` decreases reference counter. The object will be removed if the counter gets 0.
* `autorelease()` decreases reference counter, but the counter value will be checked later.

So, for every `retain()` there should be one and only one `release()` or `autorelease()` call.

The `create()` functions always call `autorelease()` for their objects (you can check the implementation in our [EnhancedSprite](https://github.com/beardog-ukr/cocos2dx-examples/blob/master/examples/AdvancedActions/Classes/EnhancedSprite.cpp) class). That means you can create the object, then do nothing with it and it will be removed automatically.

How does it work? There is a [game loop](https://gamefromscratch.com/cocos2d-x-tutorial-series-game-loop-updates-and-action-handling/) and it does something like this:
* the program sleeps for a small (dozens of milliseconds)  amount of time, then wakes up
* processes the input (keyboard, mouse etc)
* redraws the scene. During redraw it reevaluates actions, call callbacks and other stuff.
* finally, checks the reference counters for all objects; if counter is 0 the object gets deleted.

So, if the `autorelease()` (or `removeChild()`) is called then reference counter will be decreased and the object will be deleted after current iteration of the game loop.
