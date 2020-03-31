---
title: "Корисні поради по imagemagick"
date: 2019-09-29T04:25:25+03:00
draft: false
tags: ["image processing", "textures", "imagemagick"]
---

[imagemagick](https://imagemagick.org/index.php) — консольна програма для обробки зображень.

<!--more-->

Наприклад, ось так можна розрізати один файл зображення на кілька менших:
```
convert ./Skeleton_43x37.png -crop 43x37 skeleton_attack_%02d.png
```
Тут
* `./Skeleton_43x37.png` — вхідний файл
* `-crop 43x37` — розмір результату
* `skeleton_attack_%02d.png` маска для імен файлів-результатів

А ось так об'єднати кілька зображень:
```
convert +append ./skeleton_attack_00.png ./skeleton_attack_01.png ./jh.png
convert -append ./skeleton_attack_00.png ./skeleton_attack_01.png ./jv.png
```
Тут `+append` з'єднує зображення горизонтально (одне збоку від одного), а `-append` вертикально (одне над одним).


Теоретично, за допомогою `imagemagick` можна створювати гіфки, але це вимагає досить складних скриптів. Простіше використати _ffmpeg_:
```
ffmpeg -framerate 60 -pattern_type glob -i './skeleton_attack_*.png' c1.gif
```
