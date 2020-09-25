---
title: "Screen size"
date: 2020-09-25T02:30:25+03:00
draft: false
tags: ["design resolution", "Smoke Test Project"]
author: beardog-ukr
---

We have already learned now to add objects to the screen and how to do some things with them; that's enough for a simple games. But there is one more thing left unexplained about [our default program]({{< ref "first-steps.md" >}}) and this is the screen size.

<!--more-->
{{< figure src="/cocos2dx-examples/en/post/first-steps-img/first_app_screenshot.png" title="Our first program" position="center" >}}

The [default program](https://github.com/beardog-ukr/cocos2dx-examples/tree/master/examples/SmokeTest) creates a window 480x320 pixels size. This value was specified in _AppDelegate.cpp_ by following statement:

```
static cocos2d::Size designResolutionSize = cocos2d::Size(480, 320);
```

The _cocos2d-x_ developers over-engineered the default example, so you'll see some attempts to use another screen resolutions in that file. That's a [known bug](https://github.com/cocos2d/cocos2d-x/issues/18492) actually. That part of code never works and really you can delete it [like here](https://github.com/beardog-ukr/cocos2dx-examples/blob/master/examples/ActionsDemoV1/Classes/AppDelegate.cpp).

It's too complicated to handle different screen sizes correctly, especially on mobile devices. There will be another post about this [later]({{< ref "screen-resolution-02.md" >}}). For now, you should know two things:
* you can set desired screen size for Linux or Windows using `designResolutionSize` like in above example.
* we'll use absolute hard-coded values wherever we need to position elements. That's not exactly a good practice, but it will make code much simpler.
