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


ObjLoader2::ObjLoader2(const std::string filePath) {
	std::ifstream file(filePath);
	std::cout << "file read finished" << std::endl;
	std::string number;
	std::string line;

	/* read data into a string */
	if (!file.good()) {
		std::cout << "failed to open obj-file!" << std::endl;
	}
	else {
		while (file.good()) {
			getline(file, line);
			std::stringstream stream(line);
			if (line.substr(0, 2) == "v ") {
				stream.seekg(2);
				float numbers[3];
				int i = 0;
				while (getline(stream, number, ' ')) {
					numbers[i++] = atof(number.c_str());
				}
				glm::vec3 vertexData(numbers[0], numbers[1], numbers[2]);
				vertices.push_back(vertexData);
			}
			else if (line.substr(0, 2) == "vt") {
				stream.seekg(3);
				float numbers[2];
				int i = 0;
				while (getline(stream, number, ' ')) {
					numbers[i++] = atof(number.c_str());
				}
				glm::vec2 vertexData(numbers[0], numbers[1]);
				textureCoordinates.push_back(vertexData);

			}
			else if (line.substr(0, 2) == "vn") {
				stream.seekg(3);
				float numbers[3];
				int i = 0;
				while (getline(stream, number, ' ')) {
					numbers[i++] = atof(number.c_str());
				}
				glm::vec3 vertexData(numbers[0], numbers[1], numbers[2]);
				normals.push_back(vertexData);
			}
		}
		file.clear(); // reset stream
		file.seekg(0, std::ios::beg);
		while (file.good()) {
			getline(file, line);
			std::stringstream stream(line);
			if (line.substr(0, 2) == "f ") {
				// std::cout << "index data found" << std::endl;
				stream.seekg(2);
				int indices[3];
				std::string numbers;
				while (getline(stream, numbers, ' ')) { // get index entries for triangle
					std::stringstream entry(numbers);
					std::string index;
					int i = 0;
					while (getline(entry, index, '/')) { // get values of one entry
						indices[i++] = atoi(index.c_str()) - 1; // remember that obj file starts at 1!	
					}
					// put the data into objData
					objData.push_back(vertices[indices[0]].x);
					objData.push_back(vertices[indices[0]].y);
					objData.push_back(vertices[indices[0]].z);
					objData.push_back(textureCoordinates[indices[1]].x);
					objData.push_back(textureCoordinates[indices[1]].y);
					objData.push_back(normals[indices[2]].x);
					objData.push_back(normals[indices[2]].y);
					objData.push_back(normals[indices[2]].z);
				}
			}
		}

		/* indexing */
	}
	std::cout << "PARSING OBJ DATA: finished" << std::endl;
}

std::ostream& operator<<(std::ostream& os, ObjLoader2& rhs) {
	for (auto iter = rhs.getObjData().begin(); iter != rhs.getObjData().end(); ++iter) {
		auto line_iter = iter;
		while (line_iter != iter + 8)
			os << *line_iter++ << " ";
		os << std::endl;
		iter = --line_iter;
	}
	os << std::endl;
	os << "vertices: " << rhs.getVertices().size() << std::endl;
	os << "texture coordinates: " << rhs.getUVs().size() << std::endl;
	os << "total vertices: " << rhs.getObjData().size() / 8 << std::endl;
 	return os;
}





#endif
