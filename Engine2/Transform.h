#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>

class Transform
{
public:
	Transform(const glm::vec3& p = glm::vec3(), const glm::vec3& r = glm::vec3(), const glm::vec3& s = glm::vec3(1, 1, 1)) :
	pos(p),
	rot(r),
	scale(s)
	{
	};

	glm::mat4 getModel() const {
		glm::mat4 posMatrix = glm::translate(pos);
		glm::mat4 rotXMatrix = glm::rotate(rot.x, glm::vec3(1, 0, 0));
		glm::mat4 rotYMatrix = glm::rotate(rot.y, glm::vec3(0, 1, 0));
		glm::mat4 rotZMatrix = glm::rotate(rot.z, glm::vec3(0, 0, 1));
		glm::mat4 scaleMatrix = glm::scale(scale);
		glm::mat4 rotMatrix = rotZMatrix * rotYMatrix * rotXMatrix;
		return posMatrix * rotMatrix * scaleMatrix;
	}

	glm::vec3& getPos() { return pos; }
	glm::vec3& getRot() { return rot; }
	glm::vec3& getScale() { return scale; }

	glm::vec3 pos;
	glm::vec3 rot;
	glm::vec3 scale;


	
private:
};

#endif // !TRANSFORM_H
