#include <vector>
#include <iostream>

#include "Mesh.h"




Mesh::Mesh(Vertex* vertices, unsigned int numVertices)
{
	m_drawCount = numVertices;

	std::vector<glm::vec3> vertexPositions;
	std::vector<glm::vec2> textureCoordinates;
	std::vector<glm::vec3> normalVectors;

	for (unsigned int i = 0; i < numVertices; i++) {
		vertexPositions.push_back(vertices[i].getPos());
		textureCoordinates.push_back(vertices[i].getTextCoord());
		normalVectors.push_back(vertices[i].getNormal());
	}



	std::cout << normalVectors.size() << std::endl;
	
	/* Allocate and assign a Vertex Array Object to our handle */
	glGenVertexArrays(1, &m_vertexArrayObject);

	/* Bind our Vertex Array Object as the current used object */
	glBindVertexArray(m_vertexArrayObject);

	/* Allocate and assign NUM_BUFFERS Vertex Buffer Objects to our handle */
	glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);



	/* Bind our first VBO as being the active buffer and storing vertex attributes (coordinates) */
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]);

	/* Copy the vertex data to our buffer */
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(vertexPositions[0]), &vertexPositions[0], GL_STATIC_DRAW);

	/* Specify that our coordinate data is going into attribute index 0, and contains three floats per vertex */
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	/* Enable attribute index 0 as being used */
	glEnableVertexAttribArray(0);

	//glBindVertexArray(0);


	///* Bind our first VBO as being the active buffer and storing vertex attributes (coordinates) */
	//float vers[9] = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };
	//glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]);

	///* Copy the vertex data to our buffer */
	//glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), vers, GL_STATIC_DRAW);

	///* Specify that our coordinate data is going into attribute index 0, and contains three floats per vertex */
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	///* Enable attribute index 0 as being used */
	//glEnableVertexAttribArray(0);



	/* Bind our second VBO as being the active buffer and storing vertex attributes (coordinates) */
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[TEXCOORD_VB]);

	/* Copy the vertex data to our buffer */
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(textureCoordinates[0]), &textureCoordinates[0], GL_STATIC_DRAW);

	/* Specify that our coordinate data is going into attribute index 1, and contains two floats per vertex */
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	/* Enable attribute index 1 as being used */
	glEnableVertexAttribArray(1);



	/* Bind our third VBO as being the active buffer and storing vertex attributes (coordinates) */
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[NORMAL_VB]);

	/* Copy the normals data to our buffer */
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(normalVectors[0]), &normalVectors[0], GL_STATIC_DRAW);

	/* Specify that our coordinate data is going into attribute index 2, and contains three floats per vertex */
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	/* Enable attribute index 1 as being used */
	glEnableVertexAttribArray(2);

	// unbind VAO
	glBindVertexArray(0); // fuck, I'm tired!



	///* Unbind the current VAO, not sure if necessary... */
	//glBindVertexArray(NULL);
}


Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &m_vertexArrayObject);
}

void Mesh::Draw() {
	// say we want to draw this VAO
	glBindVertexArray(m_vertexArrayObject);

	// glPolygonMode(GL_FRONT, GL_LINE); // wifeframe rendering
	glDrawArrays(GL_TRIANGLES, 0, m_drawCount);
	
	// unbind and draw the next stuff stored in an VAO
	glBindVertexArray(0);
}
