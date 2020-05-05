---
title: "Робота з екранами різних розмірів"
date: 2020-05-04T15:55:25+03:00
draft: false
tags: ["design resolution", "ResolutionPolicy"]
---

В одній з [перших статей]({{< ref "screen-resolution-01.md" >}}) я рекомендував видалити з створеного _cocos_ дефолтного коду майже все, що стосується роботи з різними роздільними здатностями. Цій проблемі присвячений [окремий баг](https://github.com/cocos2d/cocos2d-x/issues/18492) в офіційному репозиторії, але у нього низький приорітет.

<!--more-->

Розробка _Cocos2d_ починалась у далекі-далекі часи, коли різновидів форматів екрану було небагато. Потім розмаїття сильно збільшилось, а на сьогоднішній день виглядає так, що всі користувачі знову ходять з більш-менш однаковими екранами. У певний момент була актуальною проблема роздільної здатності: програма мала використовувати різні набори зображень для пристроїв різних поколінь. На мій погляд, зараз такої необхідності уже немає, проте у вас може бути інша думка з цього приводу.

Теоретично, процес розробки має виглядати приблизно так:
* розробники обирають базовий формат екрану, під який розробляється програма. Це так званий _design resolution_.
* якщо при запуску на пристрої виявляється, що його екран відрізняється від  _design resolution_, згенероване програмою зображення [буде певним чином приводитись](https://docs.cocos.com/creator/manual/en/ui/multi-resolution.html
) до цього самого _design resolution_
* всередині програми не можна використовувати фіксовані числа для позначення позиції. У навчальних статтях я використовував числа, але тільки тому, що так код виглядає простіше. Насправді позиції всіх елементів потрібно обчислювати, враховуючи реальний стан справ з екраном пристрою.

У першій програмі ви могли бачити такий код:
```cpp
auto visibleSize = Director::getInstance()->getVisibleSize();
Vec2 origin      = Director::getInstance()->getVisibleOrigin();
```
`visibleSize` — це розмір тієї частини екрану, яку бачить користувач. `origin` — положення нижнього лівого кута _частини екрану, яку бачить користувач_ відносто того зображення, яке згенерувала б програма в ідеальному випадку.

Уявіть собі ось таку ситуацію (цифри на сьогоднішній день виглядають нереальними, але най буде)

{{< figure src="/cocos2dx-examples/uk/post/screen-resolution-img/def_ex_01.jpeg" title="" position="center" >}}

В даному випадку:
* розробка велась під 800x480, це той самий _design resolution_
* під час запуску виявляється, що у пристрою екран 1024x768. Після адаптації поле збільшується, але пропорції лишаються тими самими, тому з обох боків зображення обрізається.
* після адаптації `visibleSize` буде 1024x768, а `origin` стане (128:0) — позиція нижнього лівого кута червоного прямокутника на прикаді.

_Cocos2d_ пропонує кілька варіантів приведення ([ResolutionPolicy](https://docs.cocos2d-x.org/api-ref/creator/v1.0/classes/ResolutionPolicy.html)):
* `EXACT_FIT` — зображення повністю заповнює екран, пропорції ігноруются. Результат може бути розтягнено по горизонталі або вертикалі.
* `FIXED_HEIGHT` та `FIXED_WIDTH` — співвідношення сторін зберігається, зображення може обрізатись так, щоб зберегти відповідно висоту або ширину.
* `NO_BORDER` — схоже на автоматичне обирання `FIXED_HEIGHT` або `FIXED_WIDTH`
* `SHOW_ALL` — пропорції зберігаються, зображення повністю показується, але можуть бути чорні полоси по боках або зверху екрану.

Також можна реалізувати свою власну політику, як зроблено у [цій статті](https://coderwall.com/p/svldvw/configuration-of-multiple-resolution-in-cocos2d-x-3-0).

Крім того, можна не вказувати _design resolution_ а запуститись з реальним розміром екрану. Для цього в `AppDelegate.cpp` треба зробити так:
```cpp
Size realScreenSize = glview->getFrameSize();
glview->setDesignResolutionSize(realScreenSize.width, realScreenSize.height,
                                ResolutionPolicy::NO_BORDER);
```
Тут байдуже, яку _ResolutionPolicy_ вказувати, все одно запуск виконується з розміром екрану поточного пристрою.

Якщо ви захочете використовувати різні набори зображень для різних пристроїв, доведеться змінювати дефолтні шляхи, за якими завантажуються файли. Детальніше про це написано [тут](https://github.com/cocos2d/cocos2d-x/issues/18492), [тут](http://becomingindiedev.blogspot.com/2014/05/multi-resolution-support-in-ios-with.html) або у [цій статті](https://www.codeandweb.com/texturepacker/tutorials/animations-and-spritesheets-in-cocos2d-x).