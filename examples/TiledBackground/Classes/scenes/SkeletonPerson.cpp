#include "SkeletonPerson.h"

USING_NS_CC;
using namespace std;

const string idleAnimationName = "skeleton_idle";
const string moveAnimationName = "skeleton_walk";

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

float generateStartDelay() {
  static int delayValueIdx = 0;
  const int delaysCount = 4;
  const float delays[delaysCount] = {0.1, 0.2, 0.3, 0.4};
  delayValueIdx++;
  return delays[delayValueIdx];
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

SkeletonPerson::SkeletonPerson() {
  log("SkeletonPerson constructor here");
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

SkeletonPerson::~SkeletonPerson() {
  log("SkeletonPerson destructor here");
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

SkeletonPerson* SkeletonPerson::create(const Vec2& startPos, const Vec2& endPos) {
  SkeletonPerson *result = new (std::nothrow) SkeletonPerson();
  if (result && result->initSelf(startPos, endPos)) {
    result->autorelease();
    return result;
  }
  CC_SAFE_DELETE(result);
  return nullptr;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void SkeletonPerson::doWalk() {
  goingFromAtoB = !goingFromAtoB;
  MoveTo* mt = nullptr;
  if (goingFromAtoB) {
    mt = MoveTo::create(10, positionB);
  }
  else {
    mt = MoveTo::create(8, positionA);
  }

  setScaleX(goingFromAtoB ? 1 : -1);

  CallFunc* cf = CallFunc::create([this]() {
    doWalk();
  });
  runAction(Sequence::create(mt, cf, nullptr));

}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool SkeletonPerson::initSelf(const Vec2& startPos, const Vec2& endPos) {
  positionA = startPos;
  positionB = endPos;
  goingFromAtoB = false;//note, ill be changed in doWalk

  // note, it's important to init with some file,
  //animation will not be displayed correctly if this is not done
  initWithSpriteFrameName("skeleton_idle_00.png");

  setAnchorPoint(Vec2(0.5,0.5));
  setPosition(positionA);

  CallFunc* cf = CallFunc::create([this]() {
    Animation* animation = AnimationCache::getInstance()->getAnimation(moveAnimationName);
    Animate* animate = Animate::create(animation);
    RepeatForever* rf = RepeatForever::create(animate);
    runAction(rf);
    doWalk();
  });


  DelayTime* dt = DelayTime::create(generateStartDelay());

  runAction(Sequence::create(dt, cf, nullptr));

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool SkeletonPerson::loadAnimations() {
  SpriteFrameCache* const sfc = SpriteFrameCache::getInstance();

  const string plistFilename = "skeleton/skeleton_images.plist";

  sfc->addSpriteFramesWithFile(plistFilename);
  if (!sfc->isSpriteFramesWithFileLoaded(plistFilename)) {
    printf("Error while loading: %s\n", plistFilename.c_str());
    return false;
  }

  const string animationsPlistFN = "skeleton/skeleton_animations.plist";

  AnimationCache * const ac = AnimationCache::getInstance();
  ac->addAnimationsWithFile(animationsPlistFN);

  if ((ac->getAnimation(idleAnimationName) == nullptr) ||
      (ac->getAnimation(moveAnimationName) == nullptr)) {
    printf("Error while loading: %s\n", animationsPlistFN.c_str());
    return false;
  }

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool SkeletonPerson::unloadAnimations() {
  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
