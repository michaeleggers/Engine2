#include "StereoRig.h"


StereoRig::StereoRig(glm::vec3 camPos
	, glm::vec3 camTarget
	, glm::vec3 camUp
    , float iod_)
{
	setView(camPos, camTarget, camUp);
}

StereoRig::~StereoRig()
{
}
