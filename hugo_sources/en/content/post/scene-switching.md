---
title: "Switching between scenes"
date: 2020-09-26T17:30:25+03:00
draft: false
tags: ["Scene", "Director"]
author: beardog-ukr
---

The _Cocos2d_ developers wanted the internal part of the game look like theater or movie: there is [the director(https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d7/df3/classcocos2d_1_1_director.html)] and some scenes. The director orders scenes to change one another.

<!--more-->

Actually, the Director has following methods to change the scenes:
* `replaceScene()` changes current scene with a new one
* `popScene()`/`pushScene()` works with some kind of stack of scenes. _push_ freezes the current scene, adds it to the stack and then starts the new one. On the other side, _pop_ deletes the current scene and takes another one from the stack.
* `popToRootScene()` and `popToSceneStackLevel(int level)` allows switching to root or arbitrary scene.

Anyway, the new scene completely redraws everything that was on the screen before it. In most cases this behavior is good, but there are some exceptions. Let's say you want to put the game on pause or display some message. In order to make this delay more obvious for the player it will be better to show the new scene over some content from the old one.

For example, the switch may look like this:

{{< figure src="/cocos2dx-examples/en/post/scene-switching/gray_to_bases.png" title="" position="center" >}}

First there was a scene with [candle animation]({{< ref "basic-animations.md" >}}) and rotating UFO. Then it gets changed with a new scene, but this new scene has to use older background.

There is no exact method in _Cocos2d_ to get this effect. However you can achieve it in two steps: first keep an image from the old scene and then use it as a background on the new one.

Like that:
```cpp
void StaticInterruptScene::switchToInstantScene() {
    const Size visibleSize = Director::getInstance()->getVisibleSize();
    RenderTexture* rt      = RenderTexture::create(visibleSize.width,
                                                   visibleSize.height);

    rt->begin();
    this->visit();
    rt->end();
    rt->getSprite()->setAnchorPoint(Vec2(0, 0));

    Scene* ims = InstantMenuScene::create(rt);

    Director::getInstance()->pushScene(ims);
  }
}
```

Here the [RenderTexture](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d9/ddc/classcocos2d_1_1_render_texture.html) instance keeps an image generated for the old scene. The [InstantMenuScene](https://github.com/beardog-ukr/cocos2dx-examples/blob/master/examples/AnimationDemo/Classes/scenes/InstantMenuScene.cpp) is a class for a new scene; it receives stored image as a constructor parameter and uses for a background. The usage itself is quite trivial, but you can find it [here](https://github.com/beardog-ukr/cocos2dx-examples/blob/master/examples/AnimationDemo/Classes/scenes/InstantMenuScene.cpp).
