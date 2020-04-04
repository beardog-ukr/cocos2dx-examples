#pragma once

#include "cocos2d.h"

class EnhancedSprite : public cocos2d::Sprite {
public:

static EnhancedSprite* create(const std::string& filename);

protected:
EnhancedSprite();
virtual ~EnhancedSprite();

int objectId;


};