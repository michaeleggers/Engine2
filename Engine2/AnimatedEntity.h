#ifndef RENTITY_H
#define RENTITY_H

#include "Transform.h"
#include "md2Model.h"
#include "Texture.h"
#include "Entity.h"
#include "BoundingBox.h"

enum AnimationState {
	IDLE,
	RUN,
	ATTACK,
	DEATH1
};

class AnimatedEntity : public Entity
{

public:
	AnimatedEntity() {
		currentFrame = 0;
		nextFrame = 1;
		interpol = 0.0f;
		update = false;
		m_Start = 0;
		m_End = 0;

		// VAO, VBOs for the base model
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &currentVBO);
		glGenBuffers(1, &nextVBO);
		glGenBuffers(1, &textureVBO);
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, currentVBO);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, nextVBO);
		glEnableVertexAttribArray(3);
		glBindBuffer(GL_ARRAY_BUFFER, textureVBO);
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);

		m_AnimState = IDLE;
	}
	~AnimatedEntity();

	float& getInterpol() { return interpol; }

	virtual void setAnimationState(AnimationState state) { m_AnimState = state; }
	virtual AnimationState getState() { return m_AnimState; }
	virtual void animate(int start, int end, float percent);

	void render();

	int getCurrentFrame() { return currentFrame; }

protected:
	// for animated models
	int currentFrame;
	int nextFrame;
	float interpol;
	bool update;

	// this models own VAO, VBOs
	GLuint VAO;
	GLuint currentVBO;
	GLuint nextVBO;
	GLuint textureVBO;

	// check what animation is active
	int m_Start;
	int m_End;

	AnimationState m_AnimState;
};

#endif

