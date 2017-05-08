#include "Camera.h"



Camera::Camera(glm::vec3 camPos, glm::vec3 camTarget, glm::vec3 camUp)
{
	view = glm::lookAt(glm::vec3(0, 0, 3), // camera position in world space
		glm::vec3(0, 0, 0),			       // camera target in world space
		glm::vec3(0, 1, 0));			   // camera up vector in world space (to calculate right vector)

	// default projection matrix
	projection = glm::perspective(45.0f, 800.0f / 600.0f, 0.1f, 100.0f);

	camPosition = camPos;
}


Camera::~Camera()
{
}
