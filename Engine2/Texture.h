#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL2/SDL.h>
#include <string>
#include <GL/glew.h>


class Texture
{
public:
	Texture() { m_texture = 0; width = 0; height = 0; pixels = NULL; }
	Texture(const std::string& fileName);
	//Texture(const Texture& rhs);
	void Bind(unsigned int uint);
	void Unbind();

	Texture& operator=(const Texture&);

	virtual ~Texture();

private:
	/* Handle to the texture data. */
	GLuint m_texture;

	unsigned int width;
	unsigned int height;

	const void* pixels;
};

#endif
