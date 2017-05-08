#ifndef MD2MODEL_H
#define MD2MODEL_H

#include "md2Loader.h"
#include "Texture.h"
#include "Transform.h"

class md2Model
{
public:
	md2Model();
	md2Model(const std::string& model_file, const std::string& texture_file);
	md2Model(const std::string& model_file);
	virtual ~md2Model();

	void pushGPU();
	void displayMD2(int frameNumber);
	void animate(int start, int end, float interpolation);
	void bind_texture(int texture_slot);
	void displayMD2();

	modelData_t& getModelData() { return model; }

	GLuint& getVAO() { return VAO; }
	std::vector<GLuint>& getVertexVBOs() { return vertexVBOs; }
	GLuint& getNextVertexFrameVBO() { return nextVertexFrameVBO; }
	std::vector<GLuint>& getNormalVBOs() { return normalVBOs; }
	GLuint& getNextNormalFrameVBO() { return nextNormalFrameVBO; }
	GLuint& getTextureVBO() { return textureVBO; }
	int getNumPoints_ro() { return numPoints_ro; }
	const std::vector<std::vector<glm::vec3> >& getFrames() { return vertex_frames; }

private:
	void reorganize();

	modelData_t model;									// stores the actual model
	GLuint VAO;
	std::vector<GLuint> vertexVBOs;						// a dedicated VBO for each frame for the vertices
	std::vector<GLuint> normalVBOs;						// a dedicated VBO for each frame for the normals
	GLuint textureVBO;									// one vbo for the texture (all frames can use the same texture data)

	std::vector<glm::vec3> pointList_ro;				// pointList in order for GL_TRIANGLES, does not work like that...
	std::vector<glm::vec2> st_ro;						// texture coordinates matching pointList_ro
	int numPoints_ro;
	std::vector<std::vector<glm::vec3> > vertex_frames;		// stores the frames vertices seperately. this is so nice.
	std::vector<std::vector<glm::vec3> > normal_frames;		// stores the normals for each frame

	GLuint nextVertexFrameVBO;								// VBO for storing the upcoming vertices, needed for interpolation
	GLuint nextNormalFrameVBO;							// VBO for storing the upcoming normals, needed for interpolation
	bool update;										// check if the VBOs for current and next frame need to be updated

	Texture texture;									// holds texture data
};

#endif
