---
title: "Обробка подій та колбеки"
date: 2020-03-26T17:30:25+03:00
draft: false
tags: ["event", "event processing", "std::bind", "CC_CALLBACK"]
author: beardog-ukr
---

У дефолній програмі, яку було створено у [перших постах](/first-steps.md), залишився без пояснення ще один момент — це кнопка, за допомогою якої ця програма закривалась.

<!--more-->

# Використання CC_CALLBACK_1 у MenuItemImage

Сама програма, якщо пам'ятаєте, виглядала так:

{{< figure src="/cocos2dx-examples/uk/post/first-steps-img/first_app_screenshot.png" title="Тестова програма" position="center" >}}

Якщо ви мишкою натискали на кнопку в правому нижньому куті, викликався ось такий метод, який припиняв роботу програми:
```
void SmokeTestScene::menuCloseCallback(Ref *pSender) {
  // Close the cocos2d-x game scene and quit the application
  Director::getInstance()->end();
}
```

Але чому цей колбек взагалі викликається? Вся кнопка створюється ось таким чином:
```cpp
/////////////////////////////
// 2. add a menu item with "X" image, which is clicked to quit the program
//    you may modify it.

// add a "close" icon to exit the progress. it's an autorelease object
auto closeItem = MenuItemImage::create(
  "CloseNormal.png",
  "CloseSelected.png",
  CC_CALLBACK_1(SmokeTestScene::menuCloseCallback, this));

if ((closeItem == nullptr) ||
    (closeItem->getContentSize().width <= 0) ||
    (closeItem->getContentSize().height <= 0)) {
  problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
}
else {
  float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
  float y = origin.y + closeItem->getContentSize().height / 2;
  closeItem->setPosition(Vec2(x, y));
}


// create menu, it's an autorelease object
auto menu = Menu::create(closeItem, NULL);
menu->setPosition(Vec2::ZERO);
this->addChild(menu, 1);
```

Більшість цього коду нагадує конструювання складного спрайту, яке ми вже розглядали [раніше](static-img-practice-app.md). Але є один новий момент: використання макросу `CC_CALLBACK_1` для того, щоб прив'язати метод, що буде обробляти колбек, до об'єкту меню.

Макроси `CC_CALLBACK_*` у Cocos2d зустрічаються дуже часто, а реалізовані вони за допомогою магії C++11, приблизно [так](https://github.com/cocos2d/cocos2d-x/blob/v4/cocos/base/ccMacros.h):
```cpp
// new callbacks based on C++11
#define CC_CALLBACK_0(__selector__,__target__, ...) std::bind(&__selector__,__target__, ##__VA_ARGS__)
#define CC_CALLBACK_1(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, ##__VA_ARGS__)
#define CC_CALLBACK_2(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2, ##__VA_ARGS__)
#define CC_CALLBACK_3(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, ##__VA_ARGS__)
```

Функція `MenuItemImage::create` [оголошена](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d1/d30/classcocos2d_1_1_menu_item_image.html#add8af43748de9b1e9d810875ac8fea16) таким чином:
```cpp
static MenuItemImage * 	create (const std::string &normalImage, const std::string &selectedImage, const std::string &disabledImage, const ccMenuCallback &callback)
```
де `ccMenuCallback` є просто [короткою формою](https://github.com/cocos2d/cocos2d-x/blob/v4/cocos/2d/CCMenuItem.h) для `std::function`:
```cpp
typedef std::function<void(Ref*)> ccMenuCallback;
```

Таким чином, макрос `CC_CALBACK_*` отримує два параметри:
* функцію (або метод), який треба буде викликати
* об'єкт, якому належить цей метод.

Число у макросі `CC_CALBACK_*` означає кількість аргументів, які отримає функція-колбек.

Результатом є `std::function`, створений [std::bind](https://en.cppreference.com/w/cpp/utility/functional/bind). Об'єкт Cocos2d, який отримав такий колбек, збереже його і викличе, коли буде потрібно.
