#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <GL/glew.h>
#include <glm/gtx/transform.hpp>
#include <vector>

#include "Transform.h"
#include "Camera.h"
#include "Light.h"
#include "Mesh.h"
#include "md2Model.h"
#include "AnimatedEntity.h"

class AnimationShader
{

public:
	AnimationShader(const std::string& fileName, const std::vector<std::string>&);

	void Bind();
	virtual ~AnimationShader();
	void Update(Light& light, AnimatedEntity& model, Camera& camera = Camera());
	GLuint m_program;

private:
	enum {
		TRANSFORM_U,
		VIEW_U,
		PROJECTION_U,
		NORMAL_U,
		VIEWPOS,
		MAT_SHININESS,
		LIGHT_POSITION,
		LIGHT_AMBIENT,
		LIGHT_DIFFUSE,
		LIGHT_SPECULAR,
		MD2_INTERPOLATION,
		MAT_DIFFUSE,
		MAT_SPECULAR,
		NUM_UNIFORMS
	};
	static const unsigned int NUM_SHADERS = 2;
	GLuint m_shaders[NUM_SHADERS];
	GLuint m_uniforms[NUM_UNIFORMS];
	std::string LoadShader(const std::string& fileName);
	void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
	GLuint CreateShader(const std::string& text, GLenum shaderType);

};

#endif
