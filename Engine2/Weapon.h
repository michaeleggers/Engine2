#ifndef WEAPON_H
#define WEAPON_H

#include "AnimatedEntity.h"

class Weapon : public AnimatedEntity
{
public:
	Weapon();
	~Weapon();

	void setDamage(int d) { damage = d; }
	int getDamage() { return damage; }

private:
	int damage;
};

#endif