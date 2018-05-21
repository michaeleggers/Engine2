#include "Character.h"

Character::~Character()
{
}

void Character::animate(int start, int end, float percent)
{
	AnimatedEntity::animate(start, end, percent);
	if (equipped) {
		m_Weapon.animate(start, end, percent);
	}
}

void Character::render()
{
	m_TexturePtr[DIFFUSE]->Bind(0);
	AnimatedEntity::render();
	if (equipped) {
		m_Weapon.getTexturePtr(DIFFUSE)->Bind(0);
		m_Weapon.render();
	}
}
