---
title: "Видалення об'єктів"
date: 2020-04-03T13:30:25+03:00
draft: false
tags: ["child-parent", "CallFunc", "RemoveSelf", "memory management"]
author: beardog-ukr
---

Тепер, після того, як ми [ознайомились з Action]({{< ref "basic-actions.md" >}}) та [іншими базовими термінами]({{< ref "basic-actions.md" >}}), можна розбиратись з проблемою коректного видалення об'єктів, створених Cocos2d.

Для того, щоб це проілюструвати, напишемо просту сцену, у якій буде один спрайт і одна кнопка. Після натиснення на кнопку спрайт буде переміщуватись за межі екрану і там якось видалятиметься.

<!--more-->

Насправді, замість кнопки нам доведеться зробити меню з кількома варіантами видалення, і в цілому програма виглядатиме так:


В меню є такі варіанти
* нічого не робити — просто залишити виведений за межі екрану елемент як є.
* використати [RemoveSelf](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d4/d11/classcocos2d_1_1_remove_self.html) — найбільш правильний спосіб
* використати метод [Node](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d3/d82/classcocos2d_1_1_node.html) `removeChild()` — трохи складніше і може дати [певні проблеми](https://discuss.cocos2d-x.org/t/why-removefromparent-removechild-could-be-dangerous/32223)
* не видаляти об'єкт, а зберегти для повторного використання.

# Налагоджувальний клас

Для того, щоб краще було прослідкувати за життям об'єкту, будемо використовувати не простий [Sprite](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d3/d5c/classcocos2d_1_1_sprite.html), а його нащадок з додатковими налагоджувальними повідомленнями у конструкторі та деструкторі.

Реалізуватиметься він ось так:
```cpp
class EnhancedSprite : public cocos2d::Sprite {
public:
  static EnhancedSprite* create(const std::string& filename);
protected:
  EnhancedSprite();
  virtual ~EnhancedSprite();

  int objectId;
};

static int objectIdGenerator = 0;

EnhancedSprite::EnhancedSprite() {
  objectId = objectIdGenerator++;
  log("EnchancedSprite constructor here, id %02d", objectId);
}

EnhancedSprite::~EnhancedSprite() {
  log("EnchancedSprite destructor here, id %02d", objectId);
}

EnhancedSprite* EnhancedSprite::create(const std::string& filename) {
  EnhancedSprite *result = new (std::nothrow) EnhancedSprite();
  if (result && result->initWithFile(filename)) {
    result->autorelease();
    return result;
  }
  CC_SAFE_DELETE(result);
  return nullptr;
}
```

Тут у кожного об'єкту є поле `objectId` з унікальним значенням, яке дозволить відрізняти об'єкти один від одного. Все інше як у звичайного спрайту.

# Варіанти видалення

Якщо нічого не робити спрайти залишатимуться у пам'яті. Цікаво, що коли ви спробуєте закрити програму, то побачите, як викликаються деструктори. Тобто, формально це не є втратою пам'яті: невидимі об'єкти все ще залишаються на сцені і вона зберігає посилання на них. Але все одно очевидно, це погано і так робити не можна.

Найбільш зручним і правильним способом видалення є використання [RemoveSelf](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d4/d11/classcocos2d_1_1_remove_self.html) в кінці послідовності, ось таке:
```cpp
MoveTo* moveAct = MoveTo::create(5, Vec2(800, 120));
Sequence* seq = Sequence::create(moveAct, RemoveSelf::create(), nullptr);

blueShip->runAction(seq);
```
Цей код виведе спрайт за межі екрану і там буде видно, як виконається деструктор.

Такого ж ефекту можна досягти, якщо використати [CallFunc](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d3/d32/classcocos2d_1_1_call_func.html):
```cpp
MoveTo* moveAct = MoveTo::create(5, Vec2(800, 120));

Sprite* oldShip = blueShip; //save blueShip pointer because it may get changed by the time
                            // the ship reaches "out of range: destination
CallFunc* finalCf = CallFunc::create([this, oldShip]() {
  log("%s: callback implemented in processMenuRemoveChild: move finished", __func__);
  this->removeChild(oldShip);
});

Sequence* seq = Sequence::create(moveAct, finalCf, nullptr);

blueShip->runAction(seq);
```
Такий спосіб складніший, але він може знадобитись, якщо одночасно зі спрайтом треба видалити якісь об'єкти, що не мають відношення до сцени.

# Збереження для повторного використання

Припустимо, що замість видалення ви хочете зберегти об'єкт десь у [пулі](https://uk.wikipedia.org/wiki/%D0%9F%D1%83%D0%BB_%D0%BE%D0%B1%27%D1%94%D0%BA%D1%82%D1%96%D0%B2_(%D1%88%D0%B0%D0%B1%D0%BB%D0%BE%D0%BD_%D0%BF%D1%80%D0%BE%D1%94%D0%BA%D1%82%D1%83%D0%B2%D0%B0%D0%BD%D0%BD%D1%8F)). Це можливо, але доведеться зробити трохи інакше, ніж в описаних вище методах.

Усі класи cocos2dx є нащадками [Ref]() — це така спроба авторів фреймворку зробити щось схоже на Objective-C. Тому у кожного об'єкту є такі методи:
* `retain()` збільшує кількість посилань на 1, "захищаючи" об'єкт від Видалення
* `release()` зменшує кількість посилань. Якщо вона стала менше 0, об'єкт видаляється
* `autorelease()` зменшує кількість посилань, але перевірка лічильника і видалення об'єкту будуть зроблені коли-небудь потім.

Таким чином, кожному виклику `retain()` має відповідати один виклик `release()` `autorelease()`.

Для об'єктів, що створені за допомогою статичних методів `create()`, завжди викликається `autorelease()` (так само, як у нашому класі _EnhancedSprite_). Тому, якщо хочеться, можна взагалі нічого зі створеним об'єктом не робити і він видалиться автоматично.

Як це працює? Десь глибоко в основі фреймворку знаходиться ігровий цикл, у ході якого виконуються приблизно такі дії:
* програма спить дуже маленький (десятки секунд) проміжок часу, потім прокидається
* оброблює способи вводу
* сцена починає перемальовуватись (в процесі перераховуючи параметри акцій та викликаючи відповідні колбеки).
* після перемалювання сцени ті об'єкти, у яких лічильник посилань став/залишився рівним 0, видаляються.

Таким чином, якщо десь в коді викликати `autorelease()` або `removeChild()`, лічильник посилань на об'єкт зменшиться і він буде видалений по завершенню поточної ітерації ігрового циклу.
