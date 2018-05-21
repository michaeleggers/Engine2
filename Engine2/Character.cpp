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
	// TODO(Michael): checking if texture is available is not cool!
	m_TexturePtr[DIFFUSE]->Bind(0);
	if (m_TexturePtr[SPECULAR])
		m_TexturePtr[SPECULAR]->Bind(1);
	AnimatedEntity::render();
	if (equipped) {
		m_Weapon.getTexturePtr(DIFFUSE)->Bind(0);
		if (m_Weapon.getTexturePtr(SPECULAR))
			m_Weapon.getTexturePtr(SPECULAR)->Bind(1);
		m_Weapon.render();
	}
}
