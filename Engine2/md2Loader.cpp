#include "md2Loader.h"

modelData_t md2Loader::load_model(const std::string& model_file) {
	/* PARSE THE MD2 HEADER FIRST*/
	md2_header_t header;		// holds the header, is return value.
	modelData_t model;						// store the relevant model data in here and return it.
	bool valid;					// check if header starts with IDP2
								// read data to header struct
	std::ifstream input;
	input.open(model_file, std::ifstream::binary);
	int* begin = &header.ident;
	int word;
	while (input.good() && begin != header.end) {
		input.read((char*)(&word), sizeof(int));
		*begin = word;
		begin++;
	}
	// check if header is correct
	int header_check = ('2' << 24) | ('P' << 16) | ('D' << 8) | 'I';
	//printf("%04x \n", header_check); // debug
	valid = header_check == header.ident;
	input.close();
	/* END OF PARSING MD2 HEADER*/

	/* IF THE HEADER IS OK, LOAD THE MODEL */
	if (!valid) {
		std::cout << "not a valid md2 model file!" << std::endl;
	}
	else { // the yummy part begins here :)
		   // open the file (again)
		std::ifstream input;
		input.open(model_file, std::ifstream::binary);
		// get length of file:
		input.seekg(0, input.end);
		int length = input.tellg();
		input.seekg(0, input.beg);
		// allocate memory:
		char* buffer = new char[length];
		// read data as a block:
		input.read(buffer, length);
		// close the stream
		input.close();

		// some temp data we need for extracting:
		frame_t* frame;							// holds current frame metadata during looping through the file
		stIndex_t* texture_coords;				// holds current texture coordinate
		mesh_t* triangles;						// holds current index set of a triangle
		int* gl_cmds;							// holds current gl command
		std::vector<glm::vec3> framePointList;	// holds the current frame vertex data
		framePointList.reserve(header.numXYZ);	// allocate the data for one frame

												// allocate enough memory to store the vertex data for EVERY frame (one big consecutive block of vec3)
		model.pointList.reserve(header.numXYZ * header.numFrames);
		// store metadata
		model.numPoints = header.numXYZ;		// # vertices per frame
		model.numFrames = header.numFrames;		// # frames
		model.frameSize = header.frameSize;		// size for one frame in bytes
												// loop through # frames in the model
		for (int i = 0; i < header.numFrames; i++) {
			// extract the frame data
			frame = (frame_t*)&buffer[header.offsetFrames + i * header.frameSize];
			// print the frames metadata for debug
			//printf("scale: %d, translate: %d\n", frame->scale, frame->translate);
			//printf("%s\n\n", frame->name);
			// calculate vertex positions based on frame info we just got
			for (int j = 0; j < header.numXYZ; j++) {
				// reconstruct the compressed vertex data, note that we swap y and z coordinates since md2 stores them flipped
				glm::vec3 vertex(
					frame->scale[0] * frame->fp[j].v[0] + frame->translate[0],
					frame->scale[2] * frame->fp[j].v[2] + frame->translate[2],
					frame->scale[1] * frame->fp[j].v[1] + frame->translate[1]
				);
				model.pointList.push_back(vertex);
			}
		}
		// print total number of vertices including all frames for debug
		//printf("vertices: %d\n", model.pointList.size());
		//printf("vertices per frame: %d\n", header.numXYZ);
		//printf("vertices according to header: %d\n\n", header.numXYZ * header.numFrames);

		// allocate the memory for texture coordinates like we did for the vertices
		model.st.reserve(header.numST);
		// store number of texture coordinates
		model.numST = header.numST;
		// go to the place in the md2 file where the texture data begins
		texture_coords = (stIndex_t*)&buffer[header.offsetST];
		// store the texture coordinates in st
		for (int i = 0; i < header.numST; i++) {
			glm::vec2 texture_coordinate(
				texture_coords[i].s / (float)header.skinWidth,
				texture_coords[i].t / (float)header.skinHeight // t coordinate is flipped in md2
			);
			model.st.push_back(texture_coordinate);
		}
		// print total number of texture coordinates for debug
		//printf("texture coordinates: %d\n", model.st.size());
		//printf("texture coordinates according to header: %d\n\n", header.numST);

		// allocate the memory required for triangle index list
		model.triIndex.reserve(header.numTris);
		// get to the point in the file where the triangle index data is burried
		triangles = (mesh_t*)&buffer[header.offsetTris];
		// update the model info
		model.numTriangles = header.numTris;
		for (int i = 0; i < header.numTris; i++) {
			mesh_t triangle;
			triangle.meshIndex[0] = triangles[i].meshIndex[0];
			triangle.meshIndex[1] = triangles[i].meshIndex[1];
			triangle.meshIndex[2] = triangles[i].meshIndex[2];
			triangle.stIndex[0] = triangles[i].stIndex[0];
			triangle.stIndex[1] = triangles[i].stIndex[1];
			triangle.stIndex[2] = triangles[i].stIndex[2];
			model.triIndex.push_back(triangle);
		}
		// print total number of triangles for debug
		//printf("triangles: %d\n", model.triIndex.size());
		//printf("triangles according to header: %d\n\n", header.numTris);

		// allocate enough memory for storing gl commands
		model.glCommands.reserve(header.numGLcmds);
		// get to the point in the file where the gl commands are burried
		gl_cmds = (int*)&buffer[header.offsetGLcmds];
		for (int i = 0; i < header.numGLcmds; i++) {
			model.glCommands.push_back(*gl_cmds);
			gl_cmds++;
		}
		// print total numbers of glCommands for debug
		//printf("total number of gl commands: %d\n", model.glCommands.size());
		//printf("total number of gl commands according to header: %d\n\n", header.numGLcmds);

		// shutdown
		free(buffer);

		// one last thing:
		model.currentFrame = 0;
		model.nextFrame = 1;
		model.interpol = 0.0f;
	}
	return model;
}