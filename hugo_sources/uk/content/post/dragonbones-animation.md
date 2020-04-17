---
title: "Анімація з Dragon Bones"
date: 2020-04-15T17:30:25+03:00
draft: false
tags: ["Animation", "Dragon Bones"]
author: beardog-ukr
---

Існує [багато](https://www.slant.co/options/15725/alternatives/~dragonbones-pro-alternatives) різних засобів для створення скелетної анімації, але більшість з них платні, а тому недоступні для вивчення. У цій статті мова піде про [Dragon Bones](https://dragonbones.github.io/en/index.html) — доступний, але трохи своєрідний варіант.

Основною перевагою _Dragon Bones_ є те, що ця програма має достатню функціональність і при цьому є безкоштовною. Точніше, редактор є безкоштовним з закритим кодом, а код для інтеграції з _Cocos2d_ розповсюджується за ліцензією MIT.

<!--more-->

Основний недолік — китайське походження, і в даному випадку це не просто географічна характеристика. Скажімо, прямо зараз вам наврядчи вдастся скачати редактор з офіційної [сторінки завантаження](https://dragonbones.github.io/en/download.html), там виникли якісь проблеми з нововведеннями у сучасних браузерах. Код інтеграції, що знаходиться в [офіційному репозиторії](https://github.com/DragonBones/DragonBonesCPP/tree/master/Cocos2DX_3.x), вже трохи застарів, тому я б радив брати за основу для своєї розробки [ось цей](https://github.com/beardog-ukr/dragonbones-cocos2dx-v4-demo) репозиторій, там внесені усі зміни, необхідні для Cocos2d V4.

Хай там як, після подолання початкових проблем _Dragon Bones_ працює стабільно і з ним можна робити досить круті штуки. Наприклад, такі:
{{< figure src="/cocos2dx-examples/uk/post/dragon-bones-img/dragon_bones_mecha.gif" title="Приклад CoreElementGame з офіційного репозиторію" position="center" >}}

Ясно, що це просто малюнок, [тут](https://github.com/beardog-ukr/dragonbones-cocos2dx-v4-demo/blob/master/DemoProj/Resources/mecha_1502b/mecha_1502b_tex.png) цього робота можна побачити розібраним на запчастини.

Для прикладу у цій статті ми візьмемо персонажів з [безкоштовного набору](https://jordan97.itch.io/cartoon-character-kit) і дамо їм в руки [мечі](https://krukowski.itch.io/free-weapons) та [вогнепальну зброю](https://kaylousberg.itch.io/gun-assets).

Процес створення моделі та анімації я не буду описувати, тому що це скоріше справа художників, там є окремі нюанси. Можна порадити такі навчальні матеріали: [перший](http://getting-started-dragonbones.blogspot.com/2017/01/dragonbones-tutorial-part-i.html), [другий](https://habr.com/ru/post/350254/), [третій](https://topic.alibabacloud.com/a/a-tutorial-on-using-the-dragonbones-of-the-skeleton-animation-tool_8_8_31057239.html). Натомість краще сфокусуватись на особливостях, важливих для програмування.

Результатом експорту з _Dragon Bones_ до _Cocos2d_ є [три файли](https://github.com/beardog-ukr/cocos2dx-examples/tree/master/examples/DragonBonesImport/Resources/db_export): два json, що описують модель, та одне зображення-ресурс.

Імпорт моделі у програму виглядає схоже на завантаження plist-файлів:
```cpp
#include "dragonBones/cocos2dx/CCDragonBonesHeaders.h"

dragonBones::CCFactory* factory = dragonBones::CCFactory::getFactory();
factory->loadDragonBonesData("db_export/BlueWizard_ske.json", "BlueWizard");
factory->loadTextureAtlasData("db_export/BlueWizard_tex.json", "BlueWizard");

dragonBones::CCArmatureDisplay* wizard = factory->buildArmatureDisplay("WizardArmature", "BlueWizard");

wizard->setAnchorPoint(Vec2(0.5,0.5));
wizard->setScale(0.33);
wizard->setPosition(160,140);
addChild(wizard, 10);
```
[CCArmatureDisplay](https://github.com/beardog-ukr/dragonbones-cocos2dx-v4-demo/blob/master/DemoProj/Classes/dragonBones/cocos2dx/CCArmatureDisplay.h) — клас для об'єктів, з якими ви працюєте на сцені. Він є нащадком _Node_, тому з ним можна робити всі тіж самі речі, що й з іншими об'єктами _Cocos2d_.

"BlueWizard" — це щось типу тегу, який відрізняє дану завантажену модель від усіх інших. У документації цей параметр згадується як _displayName_.

"WizardArmature" — назва скелету (_armatureName_), яка вказується у редакторі, ось тут:

{{< figure src="/cocos2dx-examples/uk/post/dragon-bones-img/db_screen_01a.png" title="" position="center" >}}

Анімація викликається ось так:
```cpp
wizard->getAnimation()->play("idle01", 1);//1 to play animation once
```
Виглядатиме це так:
{{< figure src="/cocos2dx-examples/uk/post/dragon-bones-img/wizard_move.gif" title="" position="center" >}}

Частини скелету можна змінювати прямо під час виконання програми, досягти цього можна двома способами.

По-перше, редактор дозволяє в одному слоті зберегти кілька варіантів зображень. Наприкад, тут у слоті "Gun" їх три:
{{< figure src="/cocos2dx-examples/uk/post/dragon-bones-img/db_screen_02.png" title="" position="center" >}}

В такому випадку перемикання з одного зображення на інше виконується простим вказуванням індексу:
```cpp
int currentWeaponIndex;
<....>
wizard->getArmature()->getSlot("Gun")->setDisplayIndex(currentWeaponIndex);
```

По-друге, можна створити окрему модель. у якій будуть лише змінювані зображення. Тоді завантаження і використання виглядатиме складніше:
```cpp
dragonBones::CCFactory* factory = dragonBones::CCFactory::getFactory();
factory->loadDragonBonesData("db_export/MagentaKnight_ske.json", "MagentaKnight");
factory->loadTextureAtlasData("db_export/MagentaKnight_tex.json", "MagentaKnight");

factory->loadDragonBonesData("db_export/WeaponPack_ske.json", "WeaponPackMK");
factory->loadTextureAtlasData("db_export/WeaponPack_tex.json", "WeaponPackMK");

knight = factory->buildArmatureDisplay("KnightArmature", "MagentaKnight");

<..пізніше..>
factory->replaceSlotDisplay("WeaponPackMK", "WeaponPackArmature", "OneHandWeaponSlot",
                            "morning_star", knight->getArmature()->getSlot("Weapon"));
```
Тут "WeaponPackArmature" та "OneHandWeaponSlot" — ідентифікатори скелету та слоту, вказані у редакторі:
{{< figure src="/cocos2dx-examples/uk/post/dragon-bones-img/db_screen_03.png" title="" position="center" >}}

В обох випадках результат виглядатиме однаково:
{{< figure src="/cocos2dx-examples/uk/post/dragon-bones-img/weapon_change.gif" title="" position="center" >}}
