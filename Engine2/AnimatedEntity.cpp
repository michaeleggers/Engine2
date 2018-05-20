#include "AnimatedEntity.h"

AnimatedEntity::~AnimatedEntity()
{
}

void AnimatedEntity::animate(int start, int end, float percent)
{
	if (currentFrame == 0) {
		currentFrame = start;
		nextFrame = start + 1;
		interpol = 0.0f;
		update = true;
	}
	if (m_Start != start || m_End != end) { // check if the animation is interrupted
		currentFrame = start;
		nextFrame = start + 1;
		interpol = 0.0f;
		update = true;
	}
	if (interpol >= 1.0f) {
		currentFrame = nextFrame;
		nextFrame += 1;
		if (currentFrame == end) {
			nextFrame = start;
		}
		interpol = 0.0f;
		update = true;
	}

	if (update) { // update current frame and next frame IN on vertex shader
		glBindVertexArray(VAO);

		// tell what vbo to use for vertices (current frame)
		currentVBO = m_Model_ptr->getVertexVBOs()[currentFrame];
		glBindBuffer(GL_ARRAY_BUFFER, currentVBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		// tell what VBO to use for next frame
		nextVBO = m_Model_ptr->getVertexVBOs()[nextFrame];
		glBindBuffer(GL_ARRAY_BUFFER, nextVBO);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);

		// current normals-frame
		currentNormalsVBO = m_Model_ptr->getNormalVBOs()[currentFrame];
		glBindBuffer(GL_ARRAY_BUFFER, currentNormalsVBO);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

		// next normals-frame
		nextNormalsVBO = m_Model_ptr->getNormalVBOs()[nextFrame];
		glBindBuffer(GL_ARRAY_BUFFER, nextNormalsVBO);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, 0);

		// tell what vbo to use for texture coordinates
		textureVBO = m_Model_ptr->getTextureVBO();
		glBindBuffer(GL_ARRAY_BUFFER, textureVBO);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);

		update = false;
	}
	interpol += percent;
	m_Start = start;
	m_End = end;
}

void AnimatedEntity::render()
{
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, m_Model_ptr->getNumPoints_ro());
	glBindVertexArray(0);
}
