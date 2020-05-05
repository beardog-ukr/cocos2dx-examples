---
title: "Скроллінг, переміщення карти"
date: 2020-05-04T20:55:25+03:00
draft: false
tags: ["design resolution", "scrolling"]
---

Під час розробки ігор у вас може виникнути необхідність якось переміщувати карту відповідно до дій персонажу. Особливо важливо правильно реалізувати скролінг у платформерах — я б дуже рекомендував прочитати ось [цю статтю](https://www.gamasutra.com/blogs/ItayKeren/20150511/243083/Scroll_Back_The_Theory_and_Practice_of_Cameras_in_SideScrollers.php), просто для того, щоб зрозуміти всю глибину проблеми.

<!--more-->

Для прикладу візьмемо [анімацію чарівника](https://luizmelo.itch.io/evil-wizard) та проведемо його по довгому-довгому рівню, [схожому на замок](https://szadiart.itch.io/rogue-fantasy-castle).

{{< figure src="/cocos2dx-examples/uk/post/scrolling-img/dungeon02.png" title="Карта рівню в кілька разів довша за ширину екрану" position="center" >}}

У _Cocos2d_ переміщення карти (точніше, переміщення _по карті_) легко реалізувати за допомогою класу [Camera](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d6/d2b/classcocos2d_1_1_camera.html).

[Camera](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d6/d2b/classcocos2d_1_1_camera.html), є нащадком [Node](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d3/d82/classcocos2d_1_1_node.html) тому для того, щоб змінити її позицию, можна використовувати звичайні акції (в даному випадку [MoveTo](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/de/d42/classcocos2d_1_1_move_to.html)). Але, на відміну від об'єктів, які ми використовували раніше, камера живе у тривимірному просторі, тому координати вказуються трохи інакше.

Ось так виглядає приведення камери до точки, на якій знаходиться маг:
```cpp
cocos2d::Vec2 expectedMagePos; // поточна позиція мага
<....>
Camera* camera = getDefaultCamera();
const Vec3 currentCameraPos = camera->getPosition3D();
Vec3 newCameraPos = Vec3(expectedMagePos.x, expectedMagePos.y, currentCameraPos.z);
MoveTo* cameraMoveAction = MoveTo::create(1, newCameraPos);
camera->runAction(cameraMoveAction);
```
Просто задаєте нову позицію камери (висота така сама, як у старої, а координати на площині змінюються), створюєте акцію і все, скроллінг готовий. У [демонстраційній програмі](https://github.com/beardog-ukr/cocos2dx-examples/blob/master/examples/TiledBackground/Classes/scenes/HorizontalScrollScene.cpp) камера пересувається кожного разу, як маг завершує перехід у точку, що знаходиться занадто близько до краю вікна.

Результат виглядає так:
{{< figure src="/cocos2dx-examples/uk/post/scrolling-img/mage_walks.gif" title="Маг іде вправо по карті" position="center" >}}

Зі скелетами у цій програмі пов'язана пара цікавих речей. По-перше, деякі з них ходять за межами екрану, тому ресурси на обчислення їх положення витрачаються даремно. Може виникнути бажання якось вимкнути виконання акцій для об'єктів, що знаходяться далеко від гравця. Проте така дія є [передчасною оптимізацією](https://gamedev.stackexchange.com/questions/141399/why-is-it-so-bad-to-optimize-too-early), її [краще уникати](https://love2d.org/wiki/Optimising).

По-друге, об'єктів-скелетів багато, тому для управління ними краще створити окремий клас. В принципі, для мага теж треба було б зробити свій клас, але це ускладнило б код, тому у [прикладі](https://github.com/beardog-ukr/cocos2dx-examples/blob/master/examples/TiledBackground/Classes/scenes/HorizontalScrollScene.h) його координати зберігаються у класі сцени.

Можливі два варіанти:
* зробити клас скелету нащадком [cocos2d::Sprite](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d3/d5c/classcocos2d_1_1_sprite.html)
* створити клас, що буде управляти об'єктом [cocos2d::Sprite](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d3/d5c/classcocos2d_1_1_sprite.html)

Це [дискусійне](https://discuss.cocos2d-x.org/t/cocos2d-x-general-question-about-class-design-and-inheritance/24893) питання "[наслідування проти композиції](https://en.wikipedia.org/wiki/Composition_over_inheritance)", у кожного вибору є свої переваги та недоліки. У своєму прикладі я використав [наслідування](https://github.com/beardog-ukr/cocos2dx-examples/blob/master/examples/TiledBackground/Classes/scenes/SkeletonPerson.h), а поради щодо створення композиції можна знайти у [цій статті](http://becomingindiedev.blogspot.com/2015/06/correct-composition-design-in-cocos2d-x.html).
