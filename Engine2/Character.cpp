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
	// Maybe use another uniform to tell if specular map is available
	// or use whole different shader.
	m_TexturePtr[DIFFUSE]->Bind(0);
	if (m_TexturePtr[SPECULAR])
		m_TexturePtr[SPECULAR]->Bind(1);
	else
		m_TexturePtr[DIFFUSE]->Bind(1);
	AnimatedEntity::render();
	if (equipped) {
		m_Weapon.getTexturePtr(DIFFUSE)->Bind(0);
		if (m_Weapon.getTexturePtr(SPECULAR))
			m_Weapon.getTexturePtr(SPECULAR)->Bind(1);
		else
			m_Weapon.getTexturePtr(DIFFUSE)->Bind(1);
		m_Weapon.render();
	}

}
