#ifndef STEREORIG_H
#define STEREORIG_H

#include "Camera.h"

class StereoRig
{
public:
	StereoRig(glm::vec3 camPos = glm::vec3()
		, glm::vec3 camTarget = glm::vec3()
		, glm::vec3 camUp = glm::vec3(0, 1, 0)
	    , float iod_ = 0.0f);
	~StereoRig();

	// set/update view matrix
	void setView(glm::vec3& camPos, glm::vec3& camTarget, glm::vec3& camUp) {
		glm::vec3 rightCamSide = glm::normalize(glm::cross(camPos, camUp));
		glm::vec3 leftCamSide = -rightCamSide;
		left.setView(camPos + iod * leftCamSide,
			iod * leftCamSide + camTarget,
					 camUp);
		right.setView(camPos + iod * rightCamSide,
			iod * rightCamSide + camTarget,
			camUp);
		pos = camPos;
	}

	Camera left;
	Camera right;
	glm::vec3 pos;
	float iod;			// interocular distance


};

#endif