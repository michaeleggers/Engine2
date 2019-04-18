#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>


class Camera
{
public:
	Camera(glm::vec3 camPos = glm::vec3(), glm::vec3 camTarget = glm::vec3(), glm::vec3 camUp = glm::vec3(0, 1, 0));
	virtual ~Camera();

	// set/update view matrix
	void setView(glm::vec3& camPos, glm::vec3& camTarget, glm::vec3& camUp) {
		view = glm::lookAt(camPos, camTarget, camUp);
		camPosition = camPos;
	}

	glm::mat4 &getView() { return view; }

	glm::mat4 &getProjection() { return projection; }
	
	glm::vec3 &getPosition() { return camPosition; }

private:
	/* view matrix */
	glm::mat4 view;
	//glm::vec3 viewVector;

	/* projection matrix */
	glm::mat4 projection;

	/* cam position in world space */
	glm::vec3 camPosition;
};

#endif