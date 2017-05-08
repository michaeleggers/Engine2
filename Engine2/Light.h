#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
class Light
{
public:
	Light(glm::vec3 p, glm::vec3 c) : position(p), color(c) {} ;
	virtual ~Light();
	
	 glm::vec3 &getPosition() { return position; }
	 glm::vec3 &getColor() { return color; }
	void setPosition(glm::vec3& p) { position = p; }
	void setColor(glm::vec3& c) { color = c; }

private:
	glm::vec3 position;
	glm::vec3 color;
};

#endif
