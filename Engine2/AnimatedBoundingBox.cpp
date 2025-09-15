#include "AnimatedBoundingBox.h"

AnimatedBoundingBox::~AnimatedBoundingBox() {}

void AnimatedBoundingBox::animate(int start, int end, float percent) {
  if (currentFrame == 0) {
    currentFrame = start;
    nextFrame = start + 1;
    interpol = 0.0f;
    update = true;
  }
  // if(inter)
}
