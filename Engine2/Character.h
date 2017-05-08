#ifndef CHARACTER_H
#define CHARACTER_H

#include "AnimatedEntity.h"
#include "Weapon.h"

class Character : public AnimatedEntity
{
public:
	Character() : m_HitPoints(100), m_Velocity(0.03f), equipped(false), m_Direction(1, 0, 0) {}
	~Character();

	void setVelocity(float v) { m_Velocity = v; }
	void setWeapon(Weapon& weapon) { 
		m_Weapon = weapon;
		equipped = true;
	}
	void animate(int start, int end, float percent);
	void setDirection(glm::vec3& direction) { m_Direction = direction; }
	float getVelocity() { return m_Velocity; }
	glm::vec3& getDirection() { return m_Direction; }
	Weapon& getWeapon() { return m_Weapon; }
	bool isEquipped() { return equipped; }
	int getHitPoints() { return m_HitPoints; }
	void setHitPoints(int hp) { m_HitPoints = hp; }
	void render();

private:
	int m_HitPoints;
	float m_Velocity;		// translating speed in the world
	glm::vec3 m_Direction;	// direction to which the character is moving
	Weapon m_Weapon;

	bool equipped;
};

#endif