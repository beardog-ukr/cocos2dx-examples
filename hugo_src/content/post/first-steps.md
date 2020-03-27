---
title: "Знайомство"
date: 2020-03-21T17:30:25+03:00
draft: false
tags: ["initial", "installation"]
author: beardog-ukr
---

Cocos2d-x — це фреймворк для створення кросплатформенних 2d-ігор на C++. З ним можна створювати відносно прості ігри типу Flappy Bird abo Fruit Ninja, а також більш складні сайд-скроллери та навіть стратегії.

Перебування на карантині можна розглядати як привід вивчити щось нове, тому я спробую зробити серію постів, що описують основи цього [програмного каркасу](https://uk.wikipedia.org/wiki/%D0%9F%D1%80%D0%BE%D0%B3%D1%80%D0%B0%D0%BC%D0%BD%D0%B8%D0%B9_%D0%BA%D0%B0%D1%80%D0%BA%D0%B0%D1%81).

{{< figure src="/cocos2dx-examples/post/first-steps-img/battle_of_pathogens.png" title="Гра Battle of Pathogens розроблена за допомогою Cocos Creator" position="center" >}}

# Встановлення

Процес встановлення непогано описаний для різних платформ в [офіційній документації](https://docs.cocos2d-x.org/cocos2d-x/v3/en/installation/Linux.html). Також відносно свіжі інструкції можна знайти у цьому блозі:[linux](https://rezghob.com/installing-cocos2d-x-linux-mint/) або [Windows](https://rezghob.com/installing-cocos2d-x-windows/).

В принципі, доведеться зробити дві речі:
* встановити необхідні програми: g++ та все інше, що треба для написання програм на C++, також python та cmake. Для компіляції програм під Android знадобиться Android Studio, однак це окрема тема, якій коли-небудь потім буде присвячено окремий пост.
* скачати власне [фреймворк](https://cocos2d-x.org/download), розпакувати його у потрібне місце і встановити змінні оточення.

В результаті у вас повинна з'явитись можливість виконати команду `cocos` у командному рядку
```
::>cocos -v
cocos2d-x-4.0
Cocos Console 2.3

```

# Перший проект

Для перевірки, що все встановлено правильно, Cocos2d-x може створити проект за помовчанням. Для цього треба виконати команду `cocos new`:
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

Опція `-l cpp` очевидно показує, що проект буде написано на C++. Повну довідку по команді можна отримати за допомогою `cocos new --help`

В даному випадку SmokeTest — це назва проекту. Скрипт cocos створить папку з такою назвою та заповнить її початковими файлами. Переходимо у цю  новостворену папку та запускаємо компіляцію командою `cocos compile -p linux`. Результатом має бути типовий для _cmake_ лог.

Перша компіляція займає багато часу, тому що збирається весь фреймворк; пізніше буде набагато швидше.

Нарешті, запустити програму можна за допомогою `cocos run`, в результаті повинно з'явитись щось таке:

{{< figure src="/cocos2dx-examples/post/first-steps-img/first_app_screenshot.png" title="Тестова програма" position="center" >}}

У наступних постах я спробую детальніше розібрати, що тут до чого, а поки що це все, дякую за увагу.
