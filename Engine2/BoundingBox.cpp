#include "BoundingBox.h"

BoundingBox::BoundingBox()
{
}

BoundingBox::BoundingBox(md2Model& model)
{
	int numFrames = model.getFrames().size();
	int numVertices = model.getNumPoints_ro();
	m_Boxes.reserve(numFrames);

	// iterate through all the frames
	for (auto iter = model.getFrames().begin(); iter != model.getFrames().end(); ++iter) {
		// get find the x(min/max), y(min/max), z(min/max) for each frame
		float x_min = 0, x_max = 0;
		float y_min = 0, y_max = 0;
		float z_min = 0, z_max = 0;
		std::vector<glm::vec3> frame = *iter;
		for (auto iter_frame = frame.begin(); iter_frame != frame.end(); ++iter_frame) {
			// x
			if (iter_frame->x < x_min) {
				x_min = iter_frame->x;
			}
			else if (iter_frame->x > x_max) {
				x_max = iter_frame->x;
			}
			// y
			if (iter_frame->y < y_min) {
				y_min = iter_frame->y;
			}
			else if (iter_frame->y > y_max) {
				y_max = iter_frame->y;
			}
			// z
			if (iter_frame->z < z_min) {
				z_min = iter_frame->z;
			}
			else if (iter_frame->z > z_max) {
				z_max = iter_frame->z;
			}
		}
		// compute width, height, depth
		float width = x_max - x_min;
		float height = y_max - y_min;
		float depth = z_max - z_min;

		// build the box
		std::vector<glm::vec3> box = {
			glm::vec3(x_min, y_min, z_max), // bottom left front
			glm::vec3(x_max, y_min, z_max), // bottom right front
			glm::vec3(x_min, y_max, z_max), // top left front
			glm::vec3(x_max, y_max, z_max), // top right front
			glm::vec3(x_min, y_min, z_min), // bottom left back
			glm::vec3(x_max, y_min, z_min), // bottom right back
			glm::vec3(x_min, y_max, z_min), // top left back
			glm::vec3(x_max, y_max, z_min),	// top right back
			glm::vec3(width, height, depth)	// width, height, depth for this box
		};
		m_Boxes.push_back(box);
	}
}


BoundingBox::~BoundingBox()
{
}

BoundingBox& BoundingBox::operator=(const BoundingBox& rhs)
{
	m_Boxes = rhs.m_Boxes;
	return *this;
}
