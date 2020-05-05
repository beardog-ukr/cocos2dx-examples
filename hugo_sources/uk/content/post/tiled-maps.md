---
title: "Створення карт рівнів"
date: 2020-05-04T13:30:25+03:00
draft: false
tags: ["Tiled", "Tileset", "TMXMap", "third-party tools"]
author: beardog-ukr
---

Для більш-менш великих ігрових проектів, таких як стратегії або платформери, вам знадобиться окремий засіб для проектування карт рівнів. Таким засобом є [Tiled](https://www.mapeditor.org/) — універсальний редактор рівнів з відкритим кодом.
<!--more-->

# Основи

Інтерфейс Tiled в цілому схожий на інші редактори, тому я не буду тут його детально описувати (але, якщо треба, варто [глянути сюди](https://www.raywenderlich.com/2684-cocos2d-x-tile-map-tutorial-part-1)). Суть в тому, що ви берете створений художником набір елементів (tileset) і використовуєте його для побудови карти.

Для нашого першого прикладу візьмемо [цей тайлсет](http://finalbossblues.com/timefantasy/freebies/free-beach-tileset/), а також анімацію [чарівника](https://fooch.itch.io/mage) та [лицаря](https://0x72.itch.io/dungeontileset-ii).

{{< figure src="/cocos2dx-examples/uk/post/tiled-maps/beach-tiled.png" title="Робота з Tiled" position="center" >}}

Є деякі особливості, специфічні для Cocos2dx:
* один шар (layer) у редакторі має використовувати елементи [одного і того самого тайлсету](https://gamedev.stackexchange.com/questions/152458/assets-from-tiled-background-layer-not-appearing-in-cocos2d-x). Якщо ви хочете використати інший тайлсет, обов'язково треба створити новий шар.
* бажано, щоб файл тайлсету(`*.tsx`) був в одній папці з файлом карти (\*.tmx), інакше можуть виникнути проблеми під час завантаження
* старі версії Cocos2dx (3.17.2 і менше) могли працювати лише з дуже-дуже старою версією редактора. У Cocos2dx 4.0 цієї проблеми вже немає.

Карта завантажується дуже просто, фактично одним рядком:

```cpp
const char mapFilename[] = "beach/beach_map.tmx";

TMXTiledMap* mapNode = TMXTiledMap::create(mapFilename);
if (mapNode == nullptr) {
// обробка помилки  
}
addChild(mapNode, ZO_BACKGROUND);
```
`TMXTiledMap::create` створює звичайний об'єкт-нащадок [Node](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d3/d82/classcocos2d_1_1_node.html) і далі з ним можна робити все те ж саме, що з іншими, зокрема, використовувати у якості фону.

# Додаткова інформація на карті

Скоріш за все, під час створення рівня вам треба буде зберегти на карті якусь додаткову інформацію. Наприклад, вказати місце появи головного героя, а по позначити квадрати, на які герой не може заходити.

Для того, щоб це зробити, використовується такий прийом:
* на карту додають спеціальний "службовий" шар, на якому використовуються елементи зі спеціального тайлсету.
* під час завантаження інформація зі службового шару обробляється окремо, а потім його приховують від гравця.

Службовий тайлсет може виглядати так:

{{< figure src="/cocos2dx-examples/uk/post/tiled-maps/meta-code-tile.png" title="Робота з Tiled" position="center" >}}

Немає значення, що саме зображено на квадратах; в даному випадку це просто цифри і позначки.

До кожного елементу тайлсету треба додати спеціальну властивість — числовий код, за допомогою яких програма відрізнятиме тайли один від одного. На малюнку вище така властивість названа 'MetaCode', додається вона після натиснення на синій "+" внизу панелі.

Завантаження службового шару виглядає так:
```cpp
bool SimpleNoScrollScene::loadMetaInfo(TMXTiledMap* const mapNode) {

  const string metaLayerName = "meta";
  TMXLayer* const metaLayer = mapNode->getLayer(metaLayerName);
  if (metaLayer == nullptr) {
    printf("Failed to find %s layer\n", metaLayerName.c_str());
    return false;
  }

  const Size mapSize = mapNode->getMapSize();
  obstaclesMapWidth = mapSize.width;
  obstaclesMapHeight = mapSize.height;
  obstaclesMap = new bool[obstaclesMapWidth*obstaclesMapHeight];
  memset(obstaclesMap, 0, (obstaclesMapWidth*obstaclesMapHeight));

  for (int tileX = 0; tileX < mapSize.width; tileX++) {
    for (int tileY = 0; tileY < mapSize.height; tileY++) {
      const int tileGid = metaLayer->getTileGIDAt(Vec2(tileX, tileY));
      const Value prop    = mapNode->getPropertiesForGID(tileGid);

      if (prop.isNull()) {
        continue;
      }

      const ValueMap vm   = prop.asValueMap();
      const auto frez = vm.find("MetaCode");

      if (frez == vm.end()) {
        continue;
      }

      const int metaCode = frez->second.asInt();

      switch (metaCode) {
      case MMC_OBSTACLE:
        obstaclesMap[obstaclesMapWidth*(obstaclesMapHeight - tileY - 1) + tileX] = true;
        break;

      case MMC_MAGE_START:
        mageStartX   = tileX;
        mageStartY   = mapSize.height - tileY - 1;
        break;

      case MMC_KNIGHT_START:
        knightStartX   = tileX;
        knightStartY   = mapSize.height - tileY - 1;

        currentKnightX = knightStartX;
        currentKnightY = knightStartY;
        break;

        // Note there is no suitable default action here
        // default:
      }
    }
  }

  metaLayer->setVisible(false);

  return true;
}
```

Тут ми отримуємо шар методом `getLayer` і обходимо всі його квадрати. Для кожного квадрату намагаємось отримати значення параметру "MetaCode", якщо таке є. В залежності від значення, це може бути або стартове місце, або позначка квадрату, на який не можна заходити.

У Tiled квадрат з координатами "0:0" знаходиться у верхньому лівому куті карти, а вісь Y зростає зверху до низу. Це суперечить підходу Cocos2dx, у якому точка "0:0" знаходиться зліва внизу, а Y збільшується вверх. Тому при завантаженні координати Y доводиться перераховувати, використовуючи вираз типу `knightStartY = mapSize.height - tileY - 1;`.

Таким чином, можна взяти таку карту:
{{< figure src="/cocos2dx-examples/uk/post/tiled-maps/beach-meta.png" title="Робота з Tiled" position="center" >}}

В програмі вона буде виглядати так:
{{< figure src="/cocos2dx-examples/uk/post/tiled-maps/beach_k.gif" title="Лицар рухається до берега" position="center" >}}

Цифри тайлсету позначають стартові позиції мага та лицаря, хрестики — місця, де не можна ходити. Лицар запрограмований так, щоб пересуватись на один квадрат вправо, доки у наступному квадраті не буде позначки-заборони. Повний прилад можна [переглянути тут](https://github.com/beardog-ukr/cocos2dx-examples/tree/master/examples/TiledBackground).
