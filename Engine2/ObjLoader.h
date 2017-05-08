#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>



class ObjLoader {

public:
	ObjLoader(const std::string filePath);
	std::vector<glm::vec3> getVertices() { return vertices; }
	std::vector<GLuint> getIndexBuffer() { return indexBuffer; }
	std::vector<glm::vec2> getUVs() { return textureCoordinates; }

private:
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> textureCoordinates;

	std::vector<GLuint> indexBuffer;
	/* contains whole obj file data */
	std::string data;
};


ObjLoader::ObjLoader(const std::string filePath) {
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
			else if (line.substr(0, 2) == "f ") {
				// std::cout << "index data found" << std::endl;
				stream.seekg(2);
				while (getline(stream, number, ' ')) {
					//std::cout << number << std::endl;
					//std::string tmp = number.substr(0, 1);
					int pos = 0;
					while (number.at(++pos) != '/');
					GLuint vertexPos = atoi((number.substr(0, pos)).c_str());
					indexBuffer.push_back(vertexPos - 1); // index buffer starts at 0, not 1 like obj file

					//int oldPos = pos + 1;
					//while (number.at(++pos) != '/');
					//int texturePos = atoi((number.substr(oldPos, pos - oldPos)).c_str()) - 1;
					//std::cout << texturePos << std::endl;
					// //reorder vertexTexture coordinates
					//glm::vec2 buffer = textureCoordinates[vertexPos];
					//textureCoordinates[vertexPos] = textureCoordinates[texturePos];
					//textureCoordinates[texturePos] = buffer;
					

				}
			}
		}

		/* indexing */

	}
	std::cout << "PARSING OBJ DATA: finished" << std::endl;

	///* parse data from string */
	//size_t pos = 0;
	//while (pos != data.size()) {									// parse as long as not EOF
	//	if (data.at(pos) == 'v' && data.at(++pos) == ' ') {		    // vertex data detected
	//		pos++;													// move cursor out of space
	//		size_t localPos = pos;
	//		while (data.at(++localPos) != '\n');					// move local cursor to next space or end of line
	//		std::string line = data.substr(pos, localPos - pos);	// get the whole line

	//		// extract the 3 values from the string, ugly but works!
	//		int linePos;
	//		std::string xCoord = line.substr(0, linePos = line.find(" "));
	//		line.erase(0, linePos + 1);
	//		std::string yCoord = line.substr(0, linePos = line.find(" "));
	//		line.erase(0, linePos + 1);
	//		std::string zCoord = line;

	//		// put them into the vector
	//		vertices.push_back(glm::vec3(
	//			atof(xCoord.c_str()),
	//			atof(yCoord.c_str()),
	//			atof(zCoord.c_str())
	//		));

	//		pos = localPos;			
	//	}
	//	pos++;
	//}
	//std::cout << "DATA INTO VECTOR: finished" << std::endl;

	//for (auto iter = vertices.begin(); iter != vertices.end(); ++iter)
	////	std::cout << iter->x << " " << iter->y << " " << iter->z << std::endl;
	//	printf("%f %f %f\n", iter->x, iter->y, iter->z);
	//std::cout << std::endl << std::endl;

	//for (auto iter = normals.begin(); iter != normals.end(); ++iter)
	//	//	std::cout << iter->x << " " << iter->y << " " << iter->z << std::endl;
	//	printf("%f %f %f\n", iter->x, iter->y, iter->z);
	//std::cout << std::endl << std::endl;

	//for (auto iter = textureCoordinates.begin(); iter != textureCoordinates.end(); ++iter)
	//		//std::cout << iter->x << " " << iter->y << " " << iter->z << std::endl;
	//	printf("%f %f\n", iter->x, iter->y);
	//std::cout << std::endl << std::endl;

	//for (auto iter = indexBuffer_VT.begin(); iter != indexBuffer_VT.end(); ++iter)
	//		std::cout << iter->x << " " << iter->y << " " << iter->z << std::endl;
	//	printf("%d \n", *iter);
	//std::cout << std::endl << std::endl;

	//int i = 0;
	//for (auto iter = indexBuffer.begin(); iter != indexBuffer.end(); ++iter) {
	//	if (i % 3 == 0)
	//		printf("\n");
	//	//	std::cout << iter->x << " " << iter->y << " " << iter->z << std::endl;
	//	printf("%d ", *iter);
	//	i++;
	//}
	//std::cout << std::endl << std::endl;
}





#endif
