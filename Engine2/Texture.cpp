#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

#include "Texture.h"
#include <cassert>

Texture::Texture(const std::string &fileName) {
  SDL_Surface *texture = NULL;
  texture = IMG_Load(fileName.c_str());
  if (texture == NULL) {
    std::cerr << "Unable to load texture " << fileName << std::endl;
  }
  // Uint32 fmt = texture->format->format;
  SDL_Surface *formattedTexture =
      SDL_ConvertSurfaceFormat(texture, SDL_PIXELFORMAT_RGB24, 0);

  width = formattedTexture->w;
  height = formattedTexture->h;
  pixels = formattedTexture->pixels;
  glGenTextures(1, &m_texture);
  glBindTexture(GL_TEXTURE_2D, m_texture);

  glTexEnvi(GL_TEXTURE_ENV, GL_REPLACE, GL_REPLACE);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, pixels);

  SDL_FreeSurface(texture);
  SDL_FreeSurface(formattedTexture);

  glBindTexture(GL_TEXTURE_2D, 0);
}

/* copy Ctor should make a deep copy, so also copying the pixel data into a new
   buffer.
   TODO: Implement that later if I have time.
   */
// Texture::Texture(const Texture& rhs) {
//	m_texture = rhs.m_texture;
// }

Texture::~Texture() { glDeleteTextures(1, &m_texture); }

void Texture::Bind(unsigned int unit) {
  // assert(unit >= 0 && unit <= 31);
  glActiveTexture(GL_TEXTURE0 + unit);
  glBindTexture(GL_TEXTURE_2D, m_texture);
}

void Texture::Unbind() { glDisable(GL_TEXTURE_2D); }

/* OK, this is a delicate topic, because assignment in this case will
 NOT copy the pixel data in memory rather than just copy the pointer
 to it (pixels). I just want it that way.*/
Texture &Texture::operator=(const Texture &rhs) {
  width = rhs.width;
  height = rhs.height;

  glGenTextures(1, &m_texture);
  glBindTexture(GL_TEXTURE_2D, m_texture);

  glTexEnvi(GL_TEXTURE_ENV, GL_REPLACE, GL_REPLACE);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, rhs.pixels);

  glBindTexture(GL_TEXTURE_2D, 0);

  return *this;
}
