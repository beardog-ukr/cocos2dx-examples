---
title: "First steps"
date: 2020-09-20T17:30:25+03:00
draft: false
tags: ["initial", "installation"]
author: beardog-ukr
---

Cocos2d is an open source software framework. It can be used to build games, apps and other cross platform GUI based interactive programs. It has a lot of branches, but this tutorial series will focus on C++ and Cocos2d-x.

<!--more-->

{{< figure src="/cocos2dx-examples/en/post/first-steps-img/battle_of_pathogens.png" title="A game Battle of Pathogens developed with Cocos Creator" position="center" >}}

# Installation

Installation process is more or less described in the [official documentation](https://docs.cocos2d-x.org/cocos2d-x/v3/en/installation/Linux.html) so there should not be any problems. Also there are good instructions [in this blog for Linux](https://rezghob.com/installing-cocos2d-x-linux-mint/) or [here for Windows](https://rezghob.com/installing-cocos2d-x-windows/), or [here for iOS](http://indiegamecodingconfessions.blogspot.com/2019/12/cocos2d-x-bootstrapping.html).

Now there are two versions of framework: 3.17 and 4.0. Both are good, and differences are not so significant.

On Linux you'll have to perform two steps:

* install all the stuff needed for C++ development (g++), also cmake and python. You'll also need Android Studio if you want to create Android apps.
* dowload [the framework](https://cocos2d-x.org/download), unpack it wherever you want and set sum environment variables.

Finally you should be able to enter `cocos` command in command line

```
::>cocos -v
cocos2d-x-4.0
Cocos Console 2.3
```

On Windows it's more or less the same. First you install cmake and python and then [Visual Studio Community](https://visualstudio.microsoft.com/vs/community/).

# First project

In order to check that everynting was installed properly you'll have to create and build the first project.

Projects are created with `cocos new` command:

```
::>cocos new -d ./ -l cpp SmokeTest
> Copy template into /home/username/proj/cocos2d/SmokeTest
> Copying Cocos2d-x files...
> Rename project name from 'HelloCpp' to 'SmokeTest'
> Replace the project name from 'HelloCpp' to 'SmokeTest'
> Replace the project package name from 'org.cocos2dx.hellocpp' to 'org.cocos2dx.SmokeTest'
> Replace the Mac bundle id from 'org.cocos2dx.hellocpp' to 'org.cocos2dx.SmokeTest'
> Replace the iOS bundle id from 'org.cocos2dx.hellocpp' to 'org.cocos2dx.SmokeTest'
```

Obviously `-l cpp` specifies that we'll use C++. The `cocos new --help` will print help on all available options.

Here "SmokeTest" is the name of the project. The cocos script will create a folder with this name and will generate all initial files there.

## Linux compilation

Things ill be a way simpler on Linux. To compile you'll have to enter command  `cocos compile -p linux` and it will show you a typical _cmake_ output

## Windows compilation

Windows compilation depends on framework version. For 3.17.2 it will create a `SmokeTest\proj.win32\SmokeTest.sln` file and you'll just open it with Visual Studio.

However it's more complicated with 4.0 framework. First you'll have to create build directory and get there like
```
mkdir build_win && cd build_win
```

Now perform following

```
cmake .. -G"Visual Studio 16 2019" -A Win32
```

This command asks _cmake_ to use Visual Studio to generate it's project file. This particula command tries to call VS 2019. There are [other strings](https://cmake.org/cmake/help/git-stage/manual/cmake-generators.7.html#visual-studio-generators) for older VS versions. "Win32 is the architecture and as for not it's the only option [available](https://discuss.cocos2d-x.org/t/error-when-building-cpp-test-on-cocos2d-x-4-0/48640/8) for cocos.

## Result

Anyway you can launch your progma with `cocos run` command:

{{< figure src="/cocos2dx-examples/en/post/first-steps-img/first_app_screenshot.png" title="Default application" position="center" >}}
