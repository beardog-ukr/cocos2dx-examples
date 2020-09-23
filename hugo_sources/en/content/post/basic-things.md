---
title: "Cocos2d-x basic ideas"
date: 2020-09-22T17:30:25+03:00
draft: false
tags: ["initial", "Smoke Test Project"]
author: beardog-ukr
---

Earlier we learned how to create a basic _cocos2d-x_ project. Actually, the project was created by `cocos` script and all we had to do was to launch it.

Now I'm going to describe some basic _cocos2d-x_ terms and then we'll be able to look closely at the sources the `cocos` generated for us.

<!--more-->

# Scene

[Scene](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d4/d5f/classcocos2d_1_1_scene.html) is a background or a container for all other elements the player sees. A real program should contain at least two scenes: one for main menu and another one for actual game. You can switch between scenes using the [Director](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d7/df3/classcocos2d_1_1_director.html) class. The idea here is that it should look like a theater: there is a scene, and some decorations and actors on it. And the Director commands them to change when time comes. However, our first programs will contain only one scene so there will be no switching between scenes.

the most important method of the scene is _init_:
```
bool SmokeTestScene::init() {
  // 1. super init first
  if (!Scene::init()) {
    return false;
  }

  //
  <створення інших об'єктів>

  return true;
}
```

This method is used as a constructor. For some (mostly historical) reasons _cocos2d-x_ projects don't use real C++ constructors, so most of the scene objects get created during _init_.

# Node and it's subclasses

[Node](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d3/d82/classcocos2d_1_1_node.html#details) is a basic class for all the elements you can see on the scene.

All _Node_ objects have following properties:

* position
* scale
* rotation (degrees, clockwise)
* anchor point position
* content size
* opacity

Also _Node_ implements [parent-child](https://docs.cocos2d-x.org/cocos2d-x/v4/en/basic_concepts/parent_child.html) relationships between objects. One object may be assigned as parent to another one, and when parent gets destroyed all its children get destroyed too. Later there will be another post about memory usage in cocos programs, but for now that' not so important.

Three _node_ subclasses appear in default program:

* [Sprite](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d3/d5c/classcocos2d_1_1_sprite.html) shows images. Probably the most frequently used class.
* [Label](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/db/de4/classcocos2d_1_1_label.html) shows text
* [MenuItemImage](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d1/d30/classcocos2d_1_1_menu_item_image.html) can show images and is used as button.

Here are all of them:

{{< figure src="/cocos2dx-examples/uk/post/basic-things-img/basic-app.png" title="Default program" position="center" >}}

The "GL Verts / GL calls" does not belong to a scene. That's a debug information the framework generates for every window. There is a way to [hide it](https://stackoverflow.com/questions/34708592/how-do-i-hide-cocos2d-x-gl-verts-calls-labels), but for now better to let it be.

Technically the _Scene_ class also is a subclass of _Node_ but it's used differently so it's better to think they are not related.
