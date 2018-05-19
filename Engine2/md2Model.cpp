#include "md2Model.h"

#include "md2Loader.h"

static bool g_test = false;

md2Model::md2Model()
{
	VAO = 0;
	textureVBO = 0;
	vertexVBOs = std::vector<GLuint>();
	normalVBOs = std::vector<GLuint>();
}

md2Model::md2Model(const std::string & model_file, const std::string & texture_file)
{
	model = md2Loader::load_model(model_file);
	VAO = 0;
	textureVBO = 0;
	vertexVBOs = std::vector<GLuint>(model.numFrames, 0);
	normalVBOs = std::vector<GLuint>(model.numFrames, 0);
	texture = Texture(texture_file);
	reorganize();
}

md2Model::md2Model(const std::string & model_file)
{
	model = md2Loader::load_model(model_file);
	VAO = 0;
	textureVBO = 0;
	vertexVBOs = std::vector<GLuint>(model.numFrames, 0);
	normalVBOs = std::vector<GLuint>(model.numFrames, 0);
	reorganize();
}


md2Model::~md2Model()
{
}

void md2Model::reorganize() {
	pointList_ro.reserve(model.numPoints * model.frameSize);
	vertex_frames.reserve(model.numFrames);
	normal_frames.reserve(model.numFrames);
	st_ro.reserve(model.numST);
	std::vector<glm::vec3> vertex_frame;
	std::vector<glm::vec3> normal_frame;
	bool textures_reordered = false;
	bool is_frame_size_set = false;

	// for every frame
	for (int framenumber = 0; framenumber < model.numFrames; framenumber++) {

		// iterate through this frames triangles
		for (int j = 0; j < model.numTriangles; j++) {
			vertex_frame.push_back(model.pointList[framenumber * model.numPoints + model.triIndex[j].meshIndex[0]]); // 1st point of triangle
			vertex_frame.push_back(model.pointList[framenumber * model.numPoints + model.triIndex[j].meshIndex[1]]); // 2nd point of triangle
			vertex_frame.push_back(model.pointList[framenumber * model.numPoints + model.triIndex[j].meshIndex[2]]); // 3rd point of triangle

			// generate normal for this triangle
			glm::vec3 x = model.pointList[framenumber * model.numPoints + model.triIndex[j].meshIndex[1]] - model.pointList[framenumber * model.numPoints + model.triIndex[j].meshIndex[0]];
			glm::vec3 y = model.pointList[framenumber * model.numPoints + model.triIndex[j].meshIndex[2]] - model.pointList[framenumber * model.numPoints + model.triIndex[j].meshIndex[0]];
			normal_frame.push_back(glm::normalize(glm::cross(x, y)));

			if (!textures_reordered) { // only needs to be done once, as every frame uses the same
				st_ro.push_back(model.st[model.triIndex[j].stIndex[0]]); // coordinate tuple for 1st point of triangle
				st_ro.push_back(model.st[model.triIndex[j].stIndex[1]]); // 2nd
				st_ro.push_back(model.st[model.triIndex[j].stIndex[2]]); // 3rd
			}
		}
		// add this frame
		vertex_frames.push_back(vertex_frame);
		normal_frames.push_back(normal_frame);

		if (!is_frame_size_set) {
			numPoints_ro = vertex_frame.size();
			is_frame_size_set = true;
		}
		textures_reordered = true;
		vertex_frame.clear();
		normal_frame.clear();
	}
}

void md2Model::pushGPU() {
	// reorganize(); // expand the vertices according to the mesh index data provided by md2 file.

	glGenVertexArrays(1, &VAO);

	//bind buffers
	for (int i = 0; i < model.numFrames; i++) {
		glGenBuffers(1, &vertexVBOs[i]);
	}
	for (int i = 0; i < model.numFrames; i++) {
		glGenBuffers(1, &normalVBOs[i]);
	}

	glGenBuffers(1, &textureVBO);
	glGenBuffers(1, &nextVertexFrameVBO);
	glGenBuffers(1, &nextNormalFrameVBO);
	glBindVertexArray(VAO);


	// fill the VBOs with the frames and upload to GPU
	for (auto iter = vertexVBOs.begin(); iter != vertexVBOs.end(); ++iter) {
		int framenumber = std::distance(vertexVBOs.begin(), iter);
		glBindBuffer(GL_ARRAY_BUFFER, *iter);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * numPoints_ro, &vertex_frames[framenumber][0], GL_DYNAMIC_DRAW);
		//glEnableVertexAttribArray(0);
	}

	// VBO for the texture coordinates
	glBindBuffer(GL_ARRAY_BUFFER, textureVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * st_ro.size(), &st_ro[0], GL_STATIC_DRAW);
	//glEnableVertexAttribArray(1);

	// VBOs for normals
	for (auto iter = normalVBOs.begin(); iter != normalVBOs.end(); ++iter) {
		int framenumber = std::distance(normalVBOs.begin(), iter);
		glBindBuffer(GL_ARRAY_BUFFER, *iter);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * (numPoints_ro / 3), &normal_frames[framenumber][0], GL_DYNAMIC_DRAW);
		//glEnableVertexAttribArray(2);
	}

	// VBO for interpolated frames (vertices)
	glBindBuffer(GL_ARRAY_BUFFER, nextVertexFrameVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * numPoints_ro, &vertex_frames[0][0], GL_DYNAMIC_DRAW); // fill the interpolated VBO with the first frame for now.
	//glEnableVertexAttribArray(3);

	// VBO for interpolated frames (normals)
	glBindBuffer(GL_ARRAY_BUFFER, nextNormalFrameVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * (numPoints_ro / 3), &normal_frames[0][0], GL_DYNAMIC_DRAW); // fill the interpolated VBO with the first frame for now.
	//glEnableVertexAttribArray(4);

	glBindVertexArray(0);
}

void md2Model::displayMD2(int frameNumber) {
	glBindVertexArray(VAO);
	// tell what vbo to use for vertices
	glBindBuffer(GL_ARRAY_BUFFER, vertexVBOs[frameNumber]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// tell what vbo to use for texture coordinates
	glBindBuffer(GL_ARRAY_BUFFER, textureVBO);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, numPoints_ro);

	glBindVertexArray(0);
}

/* obsolete */
void md2Model::animate(int start, int end, float percent) {
	if (model.currentFrame == 0) {
		model.currentFrame = start;
		model.nextFrame = start + 1;
		model.interpol = 0.0f;
		update = true;
	}
	if (model.interpol >= 1.0f) {
		model.currentFrame = model.nextFrame;
		model.nextFrame += 1;
		if (model.currentFrame == end) {
			model.nextFrame = start;
		}
		model.interpol = 0.0f;
		update = true;
	}

	if (update) {
		glBindVertexArray(VAO);
		// tell what vbo to use for vertices (current frame)
		glBindBuffer(GL_ARRAY_BUFFER, vertexVBOs[model.currentFrame]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		// tell what VBO to use for next frame
		nextVertexFrameVBO = vertexVBOs[model.nextFrame];
		glBindBuffer(GL_ARRAY_BUFFER, nextVertexFrameVBO);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);

		// tell what vbo to use for texture coordinates
		glBindBuffer(GL_ARRAY_BUFFER, textureVBO);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);

		update = false;
	}
	model.interpol += percent;

}

void md2Model::bind_texture(int texture_slot) {
	texture.Bind(texture_slot);
}

/* obsolete */
void md2Model::displayMD2() {
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, numPoints_ro);
	glBindVertexArray(0);
}
