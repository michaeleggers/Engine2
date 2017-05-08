#ifndef ENTITY_H
#define ENTITY_H

#include "Transform.h"
#include "Texture.h"
#include "md2Model.h"
#include "BoundingBox.h"

class Entity
{
public:
	Entity() : m_Model_ptr(NULL) {}
	virtual ~Entity();
	virtual void render() {};
	virtual void setPos(glm::vec3 pos) { m_Transform.pos = pos; }
	virtual void setTexture(Texture* texture) { m_TexturePtr = texture; }
	virtual void setModel(md2Model* m) {
		m_Model_ptr = m;
		m_BBox = BoundingBox(*m_Model_ptr);
	}
	virtual Texture* getTexturePtr() { return m_TexturePtr; }
	virtual md2Model* getModelPtr() { return m_Model_ptr; }
	virtual BoundingBox& getBBox() { return m_BBox; }
	virtual Transform& getTransform() { return m_Transform; }

	virtual int getCurrentFrame() = 0;

protected:

	Transform m_Transform;
	BoundingBox m_BBox;
	Texture* m_TexturePtr;
	md2Model* m_Model_ptr;
};

#endif
