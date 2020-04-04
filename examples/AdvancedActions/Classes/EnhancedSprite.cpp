#include "EnhancedSprite.h"

USING_NS_CC;
using namespace std;

static int objectIdGenerator = 0;

// =============================================================================

EnhancedSprite::EnhancedSprite() {
  objectId = objectIdGenerator++;
  log("EnchancedSprite constructor here, id %02d", objectId);
}

// =============================================================================

EnhancedSprite::~EnhancedSprite() {
  log("EnchancedSprite destructor here, id %02d", objectId);
}

// =============================================================================


EnhancedSprite* EnhancedSprite::create(const std::string& filename) {
  EnhancedSprite *result = new (std::nothrow) EnhancedSprite();
  if (result && result->initWithFile(filename)) {
    result->autorelease();
    return result;
  }
  CC_SAFE_DELETE(result);
  return nullptr;
}

// =============================================================================