---
title: "Знайомство"
date: 2020-03-21T17:30:25+03:00
draft: false
tags: ["initial", "installation"]
---

Cocos2d-x — це фреймворк для створення кросплатформенних 2d-ігор на C++.

Перебування на карантині можна розглядати як привід вивчити щось нове, тому я спробую зробити серію постів, що описують основи цього [програмного каркасу](https://uk.wikipedia.org/wiki/%D0%9F%D1%80%D0%BE%D0%B3%D1%80%D0%B0%D0%BC%D0%BD%D0%B8%D0%B9_%D0%BA%D0%B0%D1%80%D0%BA%D0%B0%D1%81).

# Джерела інформації

Документації по Cocos2d-x порівняно небагато, але досить для вивчення. Проблема в тому, що у більшості випадків вона застаріла, тому доведеться співвідносити написане і реальний стан речей.

TBD

# Встановлення

Непогані інструкції щодо встановлення можна знайти [тут](https://rezghob.com/installing-cocos2d-x-linux-mint/) або [тут](https://docs.cocos2d-x.org/cocos2d-x/v3/en/installation/Linux.html). В принципі, особливих проблем не повинно бути, на Linux потрібно зробити дві речі:
* встановити необхідні програми: g++ та все інше, що потрібно для написання програм на C++, також python та cmake. Щоб скомпілювати версію гри під Android, знадобиться Android Studio, але це окрема тема.
* скачати власне [фреймворк](https://cocos2d-x.org/download), розпакувати його у потрібне місце і встановити змінні оточення.

В результаті у вас повинна з'явитись можливість виконати команду `cocos` у командному рядку
```
::>cocos -v
cocos2d-x-4.0
Cocos Console 2.3

```

# Перший проект

Для перевірки, що все втановлено правильно, Cocos2d-x може створити проект за помовчанням. Для цього треба виконати команду `cocos new`:
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

Опція `-l cpp` очевидно показує, що проект буде написано на C++. Повну довідку по команді можна отримати за допомогою `cocos help new`

В даному випадку SmokeTest — це назва проекту. Скрипт cocos створить папку з такою назвою та заповнить її початковими файлами. Переходимо у цю  новостворену папку та запускаємо компіляцію командою `cocos compile -p linux`. Результатом має бути типовий для _cmake_ лог.

Перша компіляція займає багато часу, тому що збирається весь фреймворк; пізніше буде набагато швидше.

Нарешті, запустити програму можна за допомогою `cocos run`, в результаті повинно з'явитись щось таке:

{{< figure src="/cocos2dx-examples/post/first-steps-img/first" title="Steve Francia" position="center" >}}
