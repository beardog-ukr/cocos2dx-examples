---
title: "imagemagick tricks"
date: 2020-09-28T04:25:25+03:00
draft: false
tags: ["image processing", "textures", "imagemagick", "ffmpeg", "gifsicle"]
---

[imagemagick](https://imagemagick.org/index.php) is a free console software to create, edit, compose, or convert bitmap images.

<!--more-->

# Split image

Sometimes frames for animated characters come joined into a single image. For example, this is the image for the [Skeleton](https://jesse-m.itch.io/skeleton-pack) death:

{{< figure src="/cocos2dx-examples/en/post/useful-imagemagick-img/Skeleton_Dead.png" title="Hit and death of a skeleton" position="center" >}}

And the following command splits this image into a series of single pictures:
```
convert ./Skeleton_43x37.png -crop 43x37 skeleton_attack_%02d.png
```
Here
* `./Skeleton_43x37.png` is an input file
* `-crop 43x37` is the size of a single image
* `skeleton_attack_%02d.png` is a mask for result file names

# Join images

That's the opposite of previous task

```
convert +append ./skeleton_attack_00.png ./skeleton_attack_01.png ./jh.png
convert -append ./skeleton_attack_00.png ./skeleton_attack_01.png ./jv.png
```
Here `+append` joins the images horizontally (into a single row), and `-append` does similar thing but vertically (into a column).

# Resize a gif

It's quite hard to [make a GIF smaller](https://www.imagemagick.org/discourse-server/viewtopic.php?t=29205), and there can be some unexpected side effects.

However, this command could do the job:
```
convert input.gif -coalesce -scale 385x334 -fuzz 2% +dither -layers Optimize scaled.gif
```

# Create an animation from files

Actually, the _imagemagick_ isn't a good tool for this task, it will be better to  use _ffmpeg_:
```
ffmpeg -framerate 60 -pattern_type glob -i './skeleton_attack_*.png' c1.gif
```

Also, the animations in [this]({{< ref "basic-actions.md" >}}) [articles]({{< ref "basic-sequence.md" >}}) were made by [Peek](https://github.com/phw/peek) tool in Ubuntu.

# Remove some sequence from animation

A simpler way to remove some frames from a GIF is to split it into separate frames, remove some and join the rest ones back. However, this does not always work.

There are some [other ways](https://askubuntu.com/questions/870855/cut-duration-of-animated-gif) to complete this task. For example, you can use the `gifsicle` in Ubuntu.

This command keeps frames from 25th to 78th into another file:
```
gifsicle ./dragon_bones_mecha_03.gif '#25-78' > zz.gif
```

And this one just removes specified frames:
```
gifsicle ./dragon_bones_mecha_03.gif --delete '#01-25' > zz01.gif
```

Also, you can convert GIF files with [this tool](https://ezgif.com/effects) online.
