---
title: "Складніша робота з об'єктами"
date: 2020-04-02T15:55:25+03:00
draft: false
tags: ["Action", "tag"]
---


У [попередній статті]({{< ref "basic-actions.md" >}}) було показано, як за допомогою [Action](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/db/d61/classcocos2d_1_1_action.html) можна ініціювати рух та інші перетворення об'єктів. Зараз ми продовжимо роботу цьому напрямі, розглядаючи складніші сценарії.

<!--more-->

# Завдання

Для початку сформулюємо завдання, яке має бути реалізоване.

Нехай потрібно створити форму, на якій буде два спрайти, вже знайомі нам зелене НЛО та синій корабель

1. Зелене НЛО має весь час злегка коливатись (обертатись) навколо власного центру
1. на сцені на рівні НЛО мають бути дві кнопки: права та ліва стрілка. При натисненні НЛО починає рухатись у відповідному напрямку, поки не дійде до крайньої позиції
1. якщо під час руху натиснули іншу кнопку, рух у поточному напрямі припиняється, починається у протилежному.
1. Синій корабель з'являється у лівій частині екану носом доверху. У правій знаходиться кнопка-стрілка.
1. при натисненні на стрілку корабель повертається і починає рух.
1. коли корабель доходить до місця призначення, він повертається носом доверху; після цього кнопка-стрілка змінюється на протилежну.
1. якщо знову натиснути на кнопку, корабель почне рухатись у іншому напрямі.

Таким чином, зелене НЛО буде поводити себе так:
{{< figure src="/cocos2dx-examples/uk/post/advanced-actions-img/ufo_move_sides.gif" title="Зелене НЛО рухається в сторони і трохи обертається" position="center" >}}

А синій корабель так:
{{< figure src="/cocos2dx-examples/uk/post/advanced-actions-img/blue_ship_move.gif" title="Рух та зупинка синього корабля" position="center" >}}

Нагадаю, що зображення для прикладів взяті безкоштовних наборів: [кнопки](https://evolutionarygames.itch.io/interface-elements-mobile-metallic) та [космічні кораблі](https://www.kenney.nl/assets/space-shooter-redux)

# Теги для Action

У класу [Action](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/db/d61/classcocos2d_1_1_action.html) є параметр _tag_ це просте число, яке за яким можна відрізнити одну акцію від іншої. Тег встановлюється метод _setTag_ або іноді передається у конструкторі акції.

Відповідно, у [Node](https://docs.cocos2d-x.org/api-ref/cplusplus/V3.12/d3/d82/classcocos2d_1_1_node.html) є ряд методів для роботи з акціями:
* `void stopActionByTag(int tag)` — зупинка акції з відповідним тегом
* `Action* getActionByTag(int tag)` — отримання вказівника на акцію
* `ssize_t getNumberOfRunningActionsByTag(int tag) const` — кількість діючих акцій

Крім того, існує синглтон [ActionManager](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d1/d88/classcocos2d_1_1_action_manager.html#details) зі схожими методами, але у документації не рекомендують ним користуватись.

У нашому прикладі теги знадобляться, щоб відділити обертання НЛО від його горизонтального руху. Перший пункт для зеленого НЛО реалізується за допомогою послідовності та акції [RepeatForever](https://docs.cocos2d-x.org/api-ref/cplusplus/V3.12/d9/d37/classcocos2d_1_1_repeat_forever.html) якось так:
```cpp
Sequence* rseq = Sequence::create(RotateBy::create(3, 30),
                                  RotateBy::create(3, -30), nullptr);

RepeatForever* reps = RepeatForever::create(rseq);
reps->setTag(AT_UFO_ROTATION);
greenUfo->runAction(reps);
```
Тут AT_UFO_ROTATION — це числова константа і тег, що позначає акцію обертання.

Запуск руху НЛО в стороно виглядає приблизно так:
```cpp
MoveBy* moveTo = MoveTo::create(time, Vec2(newX,240));
moveTo->setTag(AT_UFO_MOVING);
greenUfo->runAction(moveTo);
```
а зупинка ось так
```cpp
greenUfo->stopAllActionsByTag(AT_UFO_MOVING);
```
Тут `stopAllActionsByTag` зупиняє акцію, позначену константою-тегом `AT_UFO_MOVING`, натомість акція обертання буде продовжуватись.

У випадку синього корабля доведеться зупиняти послідовність. Це не відрізняється від одної акції, просто тег треба ставити саме на послідовність, а не на окремі акції, приблизно так:
```cpp
// створюємо ряд акцій для послідовності
///....

Sequence* seq = Sequence::create(rotateAct, moveAct, rotateBackAct, nullptr);
seq->setTag(AT_BS_MOVE);

blueShip->runAction(seq);
```

# Визначення акції

За логікою завдання для синього корабля виходить, що його рух не можна переривати у той час, коли він обертається назад під час зупинки. Для того, щоб це реалізувати, треба у колбеках перевіряти, чи не відбувається зараз ця акція.

Є два методи, які можуть показати, що певна акція триває у даний час: це методи `getNumberOfRunningActionsByTag()` та `getActionByTag()`.

Застосовуються вони однаково:
```cpp
if (blueShip->getNumberOfRunningActionsByTag(AT_BS_ROTATION_BACK)>0 ) {
  log("%s: rotating back, do not interfere with moving", __func__);
  return;
}
```
або
```cpp
if (blueShip->getActionByTag(AT_BS_ROTATION_BACK) != nullptr) {
  log("%s: rotating back, do not interfere", __func__);
  return;
}
```

# Обробка завершення акції

У завданні для синього корабля треба виконати якісь дії (заміну зображення на кнопці) одразу після виконання акції. Для цього призначений клас [CallFunc](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d3/d32/classcocos2d_1_1_call_func.html#details) — це акція, яка нічого не робить, лише викликає функцію, вказівник на яку задали у конструкторі.

Можна зробити [лямбда-вираз](https://uk.wikipedia.org/wiki/%D0%9B%D1%8F%D0%BC%D0%B1%D0%B4%D0%B0-%D0%B2%D0%B8%D1%80%D0%B0%D0%B7%D0%B8_%D1%83_%D0%A1%2B%2B), яка виконає необхідні дії при завершенні послідовності:
```cpp
CallFunc* finalCf = CallFunc::create([this]() {
  // запуск акції по зворотньому повороту корабля
  RotateTo* ra2 = RotateTo::create(3, 0);
  ra2->setTag(AT_BS_ROTATION_BACK);
  this->blueShip->runAction(ra2);

  // заміна зображень на правій кнопці
  // ......
});

Sequence* seq = Sequence::create(rotateAct, moveAct, finalCf, nullptr);
seq->setTag(AT_BS_MOVE);

blueShip->runAction(seq);
```
Нагадаю, що конструктор `Sequence` отримує акції, які треба об'єднати у послідовність, а також `nullptr` у кінці. В даному випадку ми виконуємо три дії: поворот, рух та обробку завершення.
