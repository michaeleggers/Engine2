#ifndef ANIMATEDBB_H
#define ANIMATEDBB_H

#include "BoundingBox.h"

class AnimatedBoundingBox : public BoundingBox
{
public:
	AnimatedBoundingBox() : currentFrame(0), nextFrame(1), interpol(0.0f), update(false) {}
	~AnimatedBoundingBox();

	void animate(int start, int end, float percent);

private:
	int currentFrame;
	int nextFrame;
	int interpol;
	bool update;
};

#endif
