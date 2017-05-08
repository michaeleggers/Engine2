#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>
#include <GL/glew.h>

/* obsolete */
class Vertex {
public:
	Vertex(glm::vec3 p, glm::vec2 tc, glm::vec3 n) {
		pos = p;
		texCoord = tc;
		normal = n;
	}
	glm::vec3& getPos() { return pos; }
	glm::vec2& getTextCoord() { return texCoord; }
	glm::vec3& getNormal() { return normal; }
private:
	glm::vec3 pos;
	glm::vec2 texCoord;
	glm::vec3 normal;
};

/* obsolete */
class Mesh
{
public:
	Mesh(Vertex* vertices, unsigned int numVertices);
	virtual ~Mesh();

	void Draw();

private:
	enum {
		POSITION_VB,
		TEXCOORD_VB,
		NORMAL_VB,
		NUM_BUFFERS,
	};

	// handles for one VAO and NUM_BUFFERS VBOs
	GLuint m_vertexArrayObject; // VAO, Vertex Array Objects
	GLuint m_vertexArrayBuffers[NUM_BUFFERS]; // Vertex Buffer Objects go in there
	unsigned int m_drawCount;

};

#endif

