#ifndef OBJLOADER2_H
#define OBJLOADER2_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <glm/glm.hpp>
#include <GL/glew.h>

class ObjLoader2 {

public:
	ObjLoader2(const std::string filePath);

	std::vector<glm::vec3> getVertices() { return vertices; }
	std::vector<GLuint> getIndexBuffer() { return indexBuffer; }
	std::vector<glm::vec2> getUVs() { return textureCoordinates; }
	const std::vector<float>& getObjData() { return objData; }
	friend std::ostream& operator<<(std::ostream& os, ObjLoader2& rhs);
private:
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> textureCoordinates;

	/* contains rearranged data in the following order: vertices, texture coordinates, normals */
	std::vector<float> objData;

	/* useless, sorry EBO... */
	std::vector<GLuint> indexBuffer;

	/* contains whole obj file data */
	std::string data;
};

#endif
