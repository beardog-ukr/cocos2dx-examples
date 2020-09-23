---
title: "Project structure"
date: 2020-09-22T01:30:25+03:00
draft: false
tags: ["project structure", "files", "resources"]
author: beardog-ukr
---

You may have already noticed that even the [basic]({{< ref "first-steps.md" >}}) _cocos2d-x_ project takes a lot of space on hard drive. The situation was even worse with older versions of framework; it could use almost 1GB for small project. Some people think it's not that significant, but just weird when you have a lot of projects and suddenly run out of space.

<!--more-->

A typical _cocos2d-x_  project looks like this:
```
SmokeTest
|-bin
|-Classes
  |-AppDelegate.cpp
  |-AppDelegate.h
  |-HelloWorldScene.cpp
  |-HelloWorldScene.h
|-cocos2d
  |- <a lot of files and folders>
|-linux-build
  |- <some other files and folders>
|-proj.ios_mac
|-proj.win32
|-proj.android
|-proj.linux
  |-main.cpp
|-Resources
|-CMakeLists.txt
```

Here:
* `bin` is your compilation results on Linux. The binary here is the one that will be launched by `cocos run` command. 
* `Classes` — all the files you should write for your game. Right now it contains two default classes that I'll describe in some posts later.
* `cocos2d` — framework sources. That's a copy made specially for your project and you'll probably never change it.
* `linux-build` — obviously the build files made during compilations
* `proj.ios_mac`, `proj.win32`, `proj.android`, `proj.linux` contain some platform specific files and you'll probably never change them. For example there is a `main.cpp` in `proj.linux` folder. It contains a default C++ "main" function that creates and launches some objects that actually do the job.
* `Resources` contains images, font files, sounds and other things needed for your game.

Finally, the `CMakeLists.txt` is a typical _cmake_ file for your project. If you never dealt with cmake earlier don't worry, the only important thing there is list of source files. It looks like this:

```
# add cross-platforms source files and header files
list(APPEND GAME_SOURCE
     Classes/AppDelegate.cpp
     Classes/HelloWorldScene.cpp
     )
list(APPEND GAME_HEADER
     Classes/AppDelegate.h
     Classes/HelloWorldScene.h
     )
```

And that's all for now. 

