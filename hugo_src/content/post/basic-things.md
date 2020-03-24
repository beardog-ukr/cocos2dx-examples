---
title: "Основні терміни Cocos2d-x"
date: 2020-03-23T17:30:25+03:00
draft: false
tags: ["initial", "Smoke Test Project"]
author: beardog-ukr
---

У попередніх постах мова йшла про те, як створити базовий проект програми у Cocos2d-x. Точніше, той проект створювався автоматично скриптом _cocos_, потрібно було його лише скомпілювати та запустити.

Цього разу я розкажу про основні терміни, поняття та класи фреймворку, а також поясню, як вони використовуються у тому базовому проекті.



# Scene

[Scene](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d4/d5f/classcocos2d_1_1_scene.html) — це тло, контейнер, всередині якого живуть своїм життям усі інші елементи, які користувач бачить на екрані. В реальній програмі буде мінімум дві сцени: меню та ігрове поле. Перемикання між різними сценами здійснюється за допомогою синглтону класу [Director](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d7/df3/classcocos2d_1_1_director.html). Відповідно до задуму розробників, це має нагадувати театральну сцену, декорації на якій змінюються за наказом режисера. Втім, у наших перших тестових програмах сцена буде лише одна, тому ніякого перемикання не буде.

Найважливіше у сцені — метод _init_:
```
bool SmokeTestScene::init() {
  // 1. super init first
  if (!Scene::init()) {
    return false;
  }

  //
  <створення інших об'єктів>

  return true;
}
```

Фактично, цей метод є конструктором. Використовувати реальний C++ конструктор для цього класу не дуже зручно, тому зазвичай більшість внутрішніх об'єктів сцени створюються саме під час виконання _init_.

# Node та його нащадки

[Node](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d3/d82/classcocos2d_1_1_node.html#details) — базовий клас, що реалізує спільну для об'єктів сцени функціональність.

Усі об'єкти `Node` мають такі властивості:
* позицію
* масштаб
* кут повороту (у градусах за годинниковою стрілкою)
* положення опорної точки
* розмір вмісту (contentSize)
* видимість

Крім того, `Node` реалізує відносини [parent-child](https://docs.cocos2d-x.org/cocos2d-x/v4/en/basic_concepts/parent_child.html). Можна призначити один об'єкт дитиною іншого, і тоді при видаленні батьківського об'єкту він видалить всіх своїх дітей. Пізніше буде окремий пост про роботу з пам'яттю та створення-видалення об'єктів, а поки що у тестових програмах це не так важливо.

У нашій програмі було три нащадки `Node`:
* [Sprite](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d3/d5c/classcocos2d_1_1_sprite.html) — призначений для виведення зображень
* [Label](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/db/de4/classcocos2d_1_1_label.html) — це панель для тексту
* [MenuItemImage](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d1/d30/classcocos2d_1_1_menu_item_image.html) — це один клас для виведення зображень, але на додачу ще має функціональність кнопки.

Ось всі вони у нашій дефолтній програмі:

{{< figure src="/cocos2dx-examples/post/basic-things-img/basic-app.png" title="Тестова програма" position="center" >}}

Напис "GL Verts / GL calls" не є частиною сцени. Це налагоджувальна інформація, яку фреймворк додає до вікна програми. Її можна [приховати](https://stackoverflow.com/questions/34708592/how-do-i-hide-cocos2d-x-gl-verts-calls-labels), але поки що цього робити не варто, воно ще знадобиться.

В принципі, згаданий вище клас `Scene` також є нащадком `Node`. Але він використовується трохи інакше, ніж інші, тому його краще було розглядати окремо.