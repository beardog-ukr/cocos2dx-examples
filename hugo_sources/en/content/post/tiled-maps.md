---
title: "Tiled maps"
date: 2020-09-26T20:30:25+03:00
draft: false
tags: ["Tiled", "Tileset", "TMXMap", "third-party tools"]
author: beardog-ukr
---

If you are creating some really large project like a strategy game or a platformer you'll need some tool to design levels. And this tool is probably [Tiled](https://www.mapeditor.org/) - a universal open source level editor.
<!--more-->

# Basics

The _Tiled_ interface is similar to other editors so I don't think there is any need to describe it (but you can check [this article](https://www.raywenderlich.com/2684-cocos2d-x-tile-map-tutorial-part-1) if you want). The thing is, you take some tileset created by graphics designer and use this tileset to create a map.

We'll take [this tileset](http://finalbossblues.com/timefantasy/freebies/free-beach-tileset/) with [wizard](https://fooch.itch.io/mage) and [knight](https://0x72.itch.io/dungeontileset-ii) animations for out first example.

{{< figure src="/cocos2dx-examples/en/post/tiled-maps/beach-tiled.png" title="Working with Tiled" position="center" >}}

The map design has some things special for Cocos2dx:
* one layer in the editor can utilize elements from [only one tileset](https://gamedev.stackexchange.com/questions/152458/assets-from-tiled-background-layer-not-appearing-in-cocos2d-x). If you want to use another tileset you have to create one more layer.
* it will be better to keep tileset file (`*.tsx`) in the same folder with map file(`*.tmx`); otherwise you can get some problems during loading
* older _cocos2d-x_ versions (like 3.17.2 and lower) were incompatible with newer versions of _Tiled_ editor. However, there is no such problem in Cocos2dx 4.0.

Map loading is easy, it's only one line of code:
```cpp
const char mapFilename[] = "beach/beach_map.tmx";

TMXTiledMap* mapNode = TMXTiledMap::create(mapFilename);
if (mapNode == nullptr) {
// error processing  
}
addChild(mapNode, ZO_BACKGROUND);
```
The `TMXTiledMap` is a subclass of [Node](https://docs.cocos2d-x.org/api-ref/cplusplus/v4x/d3/d82/classcocos2d_1_1_node.html) so you should already know how to work with it :)

# Meta information on the map

There is always some additional information you may want to add to the map. For example, it can be a starting point for the hero or some tiles where moving is prohibited.

In order to create such markings you can do following:
* add some special "meta" layer to the map. This layer should use elements from special "meta" tileset
* during loading your program should process all the information from the "meta" layer and then hide it from the player.

The "meta" tileset can look like this:

{{< figure src="/cocos2dx-examples/en/post/tiled-maps/meta-code-tile.png" title="Special tiles and custom property" position="center" >}}

Since players will never see this tiles, it doesn't matter what pictures are used there. Here it's just some numbers and letters.

You should add  a special integer value (tag) to each meta tile; later the program will use this values to distinguish one tile from another. On the picture below this tag's field is called "MetaCode". You can add it by pressing the blue "+" sign at the bottom of the panel.

The meta infirmation loading looks like:
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

Here we use `getLayer` to get the layer and then loop over its tiles. For each tile we try to get the "MetaCode" value (if there is any). Depending on the code value it can be either starting point (for mage or knight) or an obstacle. 

_Tiled_ uses coordinate system different from _Cocos2d-x_. The (0,0) appears in the top left corner of the map, and Y grows from top to bottom. That's why we should convert Y coordinate values with something like `knightStartY = mapSize.height - tileY - 1;`.

So, we can create a map like this:
{{< figure src="/cocos2dx-examples/en/post/tiled-maps/beach-meta.png" title="A map with markings" position="center" >}}

And that's how it will appear in the game:
{{< figure src="/cocos2dx-examples/en/post/tiled-maps/beach_k.gif" title="Knight moves towards water" position="center" >}}

Here meta tileset numbers are used to mark starting points for mage and knight and crosses mark tiles prohibited for walking. Knight moves one tile right, then checks if next move possible. You can see the whole code of the scene [in the repo](https://github.com/beardog-ukr/cocos2dx-examples/tree/master/examples/TiledBackground).

# Another editors

I should mention the _Tiled_ is not the only possible level editor. There are [some](https://gamedev.stackexchange.com/questions/225/tools-for-creating-2d-tile-based-maps) [more](https://gamedev.stackexchange.com/questions/1886/tile-based-2d-level-editor).
Also, you can always try to [create one](https://gamedevelopment.tutsplus.com/articles/make-your-life-easier-build-a-level-editor--gamedev-356) by yourself (or at least try).
