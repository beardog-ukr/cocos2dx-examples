---
title: "Використання Sprite"
date: 2020-03-23T22:30:25+03:00
draft: false
tags: ["Sprite", "Static Images"]
author: beardog-ukr
---

У цьому пості ми спробуємо створити першу власноруч написану програму. Поки що вона не буде нічого робити, ми просто візьмемо кілька зображеннь космічних кораблів з [безкоштовного набору](https://www.kenney.nl/assets/space-shooter-redux) та покажемо їх на екрані. Основна мета — навчитись працювати з класом [Sprite](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d3/d5c/classcocos2d_1_1_sprite.html).


# Координати та опорна точка

Cocos2d використовує "шкільну" систему координат:
* x змінюється по горизонталі зліва направо
* y змінюється по вертикалі знизу вверх.

Центр системи координат знаходиться в опорній точці (anchor point). Положення опорної точки можна задавати окремо для кожного об'єкту `Node` методом [setAnchorPoint](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d3/d82/classcocos2d_1_1_node.html#abf77210f31f01d0f564600c94c90e2be). Метод отримує два параметри у діапазоні від 0 до 1, найчастіше це значення 0, 0.5 або 1. Наприклад:
* setActionPoint(Vec2(0.5, 0.5)) встановлює опорну точку в центр об'єкту.
* setActionPoint(Vec2(0, 0)) встановлює опорну точку у лівий нижній кут об'єкту. Так за помовчанням робить клас Scene.
* setActionPoint(Vec2(0, 1)) встановлює опорну точку у лівий верхній кут об'єкту. Це схоже на систему координат, прийняту у [html canvas](https://www.w3schools.com/graphics/canvas_coordinates.asp) або [Qt](https://doc.qt.io/archives/qt-4.8/coordsys.html), але, на відміну від них, вісь Y все ще направлена знизу вверх.

# Програма з фоном

Щоб побачити. як це все працює, ми візьмемо малюнок, що складається з клітин розміром 160х120 пікселів і використаємо його, як фонове зображення для програми.

{{< figure src="/cocos2dx-examples/post/static-img-practice-app/empty_grid.png" title="Пуста сітка, уявні вісі, розміри" position="center" >}}

Робиться це за допомогою такого коду, розміщеного у фунції `init` сцени:
```
const char backFilename[] = "background.png";

Sprite* sprite = Sprite::create(backFilename);
if (sprite == nullptr) {
  printf("Error while loading: %s\n", backFilename);
  return false;
}

sprite->setAnchorPoint(Vec2(0,0));
sprite->setPosition(0,0);
addChild(sprite, 0);
```

Спрайт створюється за допомогою статичного методу [create](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d3/d5c/classcocos2d_1_1_sprite.html#a3f75a553fe02f3de82042e81b76da010). Це стандартна для Cocos2d практика — майже у кожного класу є такий метод, за допомогою якого і треба створювати об'єкти.

Потім ми встановлюємо опорну точку спрайту у лівий нижній кут і ставимо спрайт на центр координат сцени (який теж знаходиться у лівому нижньому куті).

Метод `addChild` додає новостворений спрайт на сцену. Другим параметром цього методу є [z-order](https://docs.cocos2d-x.org/cocos2d-x/v4/en/basic_concepts/scene.html): якщо без подробиць, це число, яке визначає порядок "перекривання" елементів. Елементи з більшим числом будуть намальовані пізніше, відповідно, менші стануть для них фоном.

# Більше спрайтів

Тепер додамо до сцени ще пару спрайтів. Один за допомогою ось такого коду:
```cpp
const char shipFilename[] = "ships/playerShip3_blue.png";

Sprite* sprite = Sprite::create(shipFilename);

sprite->setAnchorPoint(Vec2(0.5,0.5));
sprite->setPosition(Vec2(160, 240));

addChild(sprite, 10);
```
А інший ось так:
```cpp
const char shipFilename[] = "ships/playerShip3_blue.png";
Sprite* sprite = Sprite::create(shipFilename);

sprite->setRotation(45);
sprite->setPosition(Vec2(160, 120));

addChild(sprite, 10);
```

Відмінність у тому, що в другому випадку спрайт додатково повертається на 45 градусів.

В обох спрайтів опорна точка знаходиться в центрі (0.5, 0.5). Відповідно, після встановлення позиції центр спрайту опиниться на перетині квадратів, ось так:

{{< figure src="/cocos2dx-examples/post/static-img-practice-app/normal.png" title="Сині кораблі, опорна точка в центрі" position="center" >}}

Створимо ще два спрайти, ось таким чином:
```cpp
const char shipFilename[] = "ships/playerShip3_green.png";
Sprite* sprite = Sprite::create(shipFilename);

sprite->setAnchorPoint(Vec2(0,0));
sprite->setPosition(Vec2(480, 240));

addChild(sprite, 10);

```
і таким:
```cpp
const char shipFilename[] = "ships/playerShip3_green.png";
Sprite* sprite = Sprite::create(shipFilename);

sprite->setAnchorPoint(Vec2(0,0));
sprite->setRotation(45);
sprite->setPosition(Vec2(480, 120));

addChild(sprite, 10);
```

Тут опорна точка спрайтів встановлюється у нижній лівий кут, також другий спрайт повертається на 45 градусів.

Результат виглядає таким чином:

{{< figure src="/cocos2dx-examples/post/static-img-practice-app/normal.png" title="Зелені кораблі, опорна точка збоку внизу" position="center" >}}

Опорна точка у зелених кораблів знаходиться на перетині квадратів, тому самі спрайти опинились правіше і вище. У другому випадку поворот здійснено навколо опорної точки.

# Node

Обє'кти класу Node можна використовувати для групування об'єктів. Наприклад, можна зібрати власний корабель, скомбінувавши три зображення:

{{< figure src="/cocos2dx-examples/post/static-img-practice-app/mixproj.png" title="Зелені кораблі, опорна точка збоку внизу" position="center" >}}

Для цього потрібен такий код:
```cpp
const char cockpitFilename[] = "parts/cockpitRed.png";
Sprite* cockpitSprite = Sprite::create(cockpitFilename);

const char wingFilename[] = "parts/wingRed.png";
Sprite* rightWingSprite = Sprite::create(wingFilename);
Sprite* leftWingSprite = Sprite::create(wingFilename);

Node* resultNode = Node::create();
resultNode->setAnchorPoint(Vec2(0.5,0.5));

//
rightWingSprite->setAnchorPoint(Vec2(0.5,0.5));
rightWingSprite->setScale(1, -1);
rightWingSprite->setPosition(Vec2(60, 15));
rightWingSprite->setRotation(90);
resultNode->addChild(rightWingSprite);

leftWingSprite->setAnchorPoint(Vec2(0.5,0.5));
leftWingSprite->setPosition(Vec2(-60, 15));
leftWingSprite->setRotation(90);
resultNode->addChild(leftWingSprite);

resultNode->addChild(cockpitSprite);
//
resultNode->setAnchorPoint(Vec2(0.5,0.5));
```
Спершу ми створюємо три спрайти (два крила та фюзеляж) і порожній об'єкт `Node`, який об'єднає ці елементи.

Спрайти крил повертаються та встановлюються у потрібне положення. Координати для їх встановлення (60, 15) підбираються емпірично, так, що більш-менш гарно виглядало. Крім того, спрайт правого крила треба відзеркалити викликом `setScale(1, -1)`.

В кінці окремі спрайти додаються у якості дітей до вузла-результату. Робота з фінальним об'єктом не відрізняється від роботи з одиноким спрайтом, тому я тут не буду наводити цей код. Як і все інше, його можна знайти у [проекті-прикладі на гітхабі](https://github.com/beardog-ukr/cocos2dx-examples/blob/master/examples/StaticSprites/Classes/StaticSpritesScene.cpp).

В результаті наша тестова програма стане такою:

{{< figure src="/cocos2dx-examples/post/static-img-practice-app/mixed.png" title="Червоні кораблі є комбінацією елементів, але з ними можна працювати, як з єдиним цілим" position="center" >}}

На цьому поки що все, дякую за увагу. У наступних постах зображення почнуть рухатись, стане цікавіше :)
