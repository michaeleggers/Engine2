#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include <glm/glm.hpp>

#include "md2Model.h"

class BoundingBox
{
public:
	BoundingBox();
	BoundingBox(md2Model& model);
	virtual ~BoundingBox();

	BoundingBox& operator=(const BoundingBox& rhs);

	float getMin_x(int frameNum) { return m_Boxes[frameNum][0].x; }
	float getMax_x(int frameNum) { return m_Boxes[frameNum][1].x; }
	float getMin_y(int frameNum) { return m_Boxes[frameNum][0].y; }
	float getMax_y(int frameNum) { return m_Boxes[frameNum][2].y; }
	float getMin_z(int frameNum) { return m_Boxes[frameNum][4].z; }
	float getMax_z(int frameNum) { return m_Boxes[frameNum][0].z; }
	float getWidth(int frameNum) { return m_Boxes[frameNum][8].x; }
	float getHeight(int frameNum) { return m_Boxes[frameNum][8].y; }
	float getDepth(int frameNum) { return m_Boxes[frameNum][8].z; }

	void scale(glm::vec3 s) {
		glm::mat4 scaleMatrix = glm::scale(s);
		for (auto frames_iter = m_Boxes.begin(); frames_iter != m_Boxes.end(); ++frames_iter) {
			for (auto frame_iter = frames_iter->begin(); frame_iter != frames_iter->end(); ++frame_iter) {
				*frame_iter = glm::vec4(frame_iter->x, frame_iter->y, frame_iter->z, 1.0f) * scaleMatrix;
			}

		}
	}

protected:
	std::vector<std::vector<glm::vec3> > m_Boxes; // bounding box for each keyframe
};

#endif
