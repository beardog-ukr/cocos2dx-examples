---
title: "Корисні поради по imagemagick"
date: 2019-09-29T04:25:25+03:00
draft: false
tags: ["image processing", "textures", "imagemagick", "ffmpeg", "gifsicle"]
---

[imagemagick](https://imagemagick.org/index.php) — консольна програма для обробки зображень.

<!--more-->

# Розділити зображення

Наприклад, ось так можна розрізати один файл зображення на кілька менших:
```
convert ./Skeleton_43x37.png -crop 43x37 skeleton_attack_%02d.png
```
Тут
* `./Skeleton_43x37.png` — вхідний файл
* `-crop 43x37` — розмір результату
* `skeleton_attack_%02d.png` маска для імен файлів-результатів

# Об'єднати кілька зображень

```
convert +append ./skeleton_attack_00.png ./skeleton_attack_01.png ./jh.png
convert -append ./skeleton_attack_00.png ./skeleton_attack_01.png ./jv.png
```
Тут `+append` з'єднує зображення горизонтально (одне збоку від одного), а `-append` вертикально (одне над одним).

# Змінити розмір анімації

Зменшення gif-файлу є досить [складним завданням](https://www.imagemagick.org/discourse-server/viewtopic.php?t=29205), з неочікуваними особливостями

```
convert input.gif -coalesce -scale 385x334 -fuzz 2% +dither -layers Optimize scaled.gif
```

# Створення анімації

Теоретично, за допомогою `imagemagick` можна створювати гіфки, але це вимагає досить складних скриптів. Простіше використати _ffmpeg_:
```
ffmpeg -framerate 60 -pattern_type glob -i './skeleton_attack_*.png' c1.gif
```

Анімовані скріншоти у [цих]({{< ref "basic-actions.md" >}}) [постах]({{< ref "basic-sequence.md" >}}) зроблені за допомогою [Peek](https://github.com/phw/peek) в Ubuntu.

# Вирізати певну послідовність з анімації

Теоретично, щоб вирізати певну послідовність кадрів з гіфки, можна перетворити її на ряд кадрів, видалити непотрібні та склеїти назад.

Проте є [простіші способи](https://askubuntu.com/questions/870855/cut-duration-of-animated-gif), напкриклад, зі спеціальною утилітою `gifsicle`.

Така команда зберігає кадри з 25-го по 78-й у іншу гіфку:
```
gifsicle ./dragon_bones_mecha_03.gif '#25-78' > zz.gif
```

А ось така просто видаляє вказаний діапазон кадрів:
```
gifsicle ./dragon_bones_mecha_03.gif --delete '#01-25' > zz01.gif
```

Також різні перетворення гіфок можна виконати [цим онлайновим інструментом](https://ezgif.com/effects).
