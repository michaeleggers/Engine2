#ifndef MD2LOADER_H
#define MD2LOADER_H

#include <glm/gtx/rotate_vector.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <vector>

// header file data
struct md2_header_t {
	int ident;
	int version;
	int skinWidth;
	int skinHeight;
	int frameSize;
	int numSkins;
	int numXYZ;
	int numST;
	int numTris;
	int numGLcmds;
	int numFrames;
	int offsetSkins;
	int offsetST;
	int offsetTris;
	int offsetFrames;		// starting point in the file for the vertex list. frames are then offsetFrames + frameNmbr * frameSize
	int offsetGLcmds;
	int offsetEnd;
	int end[1];				// not part of the header. Just a helper to find the end of this struct while iterating
};

// texture coordinates. divide by skinWidth and skinHeight respectively to restore floating point texture pos
struct stIndex_t {
	short s;
	short t;
};

// info for single frame point
struct framePoint_t {
	unsigned char v[3];		   // vertex data in compressed 8bit form. frame header used to decode
	unsigned char normalIndex; // not used here
};

// holds info for one frame, tells us how to uncompress the vertex information
struct frame_t {
	float scale[3];			// scaling for frame vertices
	float translate[3];		// translation for frame vertices
	char name[16];			// name of model
	framePoint_t fp[1];		// beginning of frame vertex list
};

/* Index data for a single triangle. The indices for each frame are the same. That means that the triangles stay
intact during animation frames, they just alter their position. */
struct  mesh_t {
	unsigned short meshIndex[3];	// vertex indices
	unsigned short stIndex[3];		// texture-coordinate indices
};

// holds texture data
struct texture_t {
	int width;				// width of texture
	int height;				// height of texture
	long int scaledWidth;	// scaled width/height for PCX images
	long int scaledHeight;
	unsigned int texID;		// texture object ID
	unsigned char* data;	// actual texture data
	unsigned char* palette;	// palette for this texture
};

/* model data data structure, that will eventually hold all the relevant data for a md2 model. */
struct modelData_t {
	int numFrames;						// number of frames
	int numPoints;						// number of vertices
	int numTriangles;					// number of triangles
	int numST;							// number of texture coordinates
	int frameSize;						// size of each frame in bytes
	int textWidth;						// texture width
	int textHeight;						// texture height
	int currentFrame;					// current frame no in animation
	int nextFrame;						// next frame no in animation
	float interpol;						// percent through current frame
	std::vector<mesh_t> triIndex;		// triangle list
	std::vector<glm::vec2> st;			// texture coordinate list
	std::vector<glm::vec3> pointList;	// vertex list of ALL frames! (huge block of data)
	std::vector<texture_t> modelTex;	// texture data
	std::vector<int> glCommands;		// array with the gl commands
};

class md2Loader {

public:
	md2Loader() {};
	static modelData_t load_model(const std::string&);
};





#endif
