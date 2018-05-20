#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/glm.hpp>

#include "AnimationShader.h"
#include "Camera.h"
#include "AnimatedEntity.h"


std::string AnimationShader::LoadShader(const std::string& fileName)
{
	std::ifstream file;
	file.open((fileName).c_str());

	std::string output;
	std::string line;

	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, line);
			output.append(line + "\n");
		}
	}
	else
	{
		std::cerr << "Unable to load shader: " << fileName << std::endl;
	}

	return output;
}

void AnimationShader::CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
{
	GLint success = 0;
	GLchar error[1024] = { 0 };

	if (isProgram)
		glGetProgramiv(shader, flag, &success);
	else
		glGetShaderiv(shader, flag, &success);

	if (success == GL_FALSE)
	{
		if (isProgram)
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		else
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);

		std::cerr << errorMessage << ": '" << error << "'" << std::endl;
	}
}

GLuint AnimationShader::CreateShader(const std::string& text, GLenum shaderType) {
	const char* shaderCode = text.c_str();
	const GLint shaderCodeLength = text.length();
	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderCode, &shaderCodeLength); // expects an array of strings, thus pass a pointer of shader_code
	glCompileShader(shader);

	CheckShaderError(shader, GL_COMPILE_STATUS, false, "ERROR: Shader compilation failed!");
	return shader;
}

AnimationShader::AnimationShader(const std::string& fileName, const std::vector<std::string>& variables)
{
	m_program = glCreateProgram();
	m_shaders[0] = CreateShader(LoadShader(fileName + ".vs"), GL_VERTEX_SHADER);
	m_shaders[1] = CreateShader(LoadShader(fileName + ".fs"), GL_FRAGMENT_SHADER);
	for (int i = 0; i < NUM_SHADERS; i++) {
		glAttachShader(m_program, m_shaders[i]);
	}

	// has to be done before calling glLinkProgram.
	for (auto iter = variables.begin(); iter != variables.end(); ++iter) {
		// params: shader program, attribute location has to match vbo index, name of variable in shader
		glBindAttribLocation(m_program, std::distance(variables.begin(), iter), (*iter).c_str());
	}



	/* Link our program */
	/* At this stage, the vertex and fragment programs are inspected, optimized and a binary code is generated for the shader. */
	/* The binary code is uploaded to the GPU, if there is no error. */
	glLinkProgram(m_program);

	CheckShaderError(m_program, GL_LINK_STATUS, true, "ERROR: Shader linking failed!");
	glValidateProgram(m_program);
	CheckShaderError(m_program, GL_VALIDATE_STATUS, true, "ERROR: Shader program is invalid!");

	m_uniforms[TRANSFORM_U] = glGetUniformLocation(m_program, "transform");
	m_uniforms[VIEW_U] = glGetUniformLocation(m_program, "view");
	m_uniforms[PROJECTION_U] = glGetUniformLocation(m_program, "projection");

	/* normal matrix, calculating the normal matrix in the vertex shader is not possible due to old GLSL support by GPU */
	m_uniforms[NORMAL_U] = glGetUniformLocation(m_program, "normalMatrix");

	// Don't forget to 'use' the corresponding shader program first (to set the uniform) THIS IS ALSO UGLY SINCE NOT EVERY SHADER HAS THIS! (but maybe ok for me :)
	m_uniforms[VIEWPOS] = glGetUniformLocation(m_program, "viewPos");
	m_uniforms[MAT_SHININESS] = glGetUniformLocation(m_program, "material.shininess");

	m_uniforms[LIGHT_POSITION] = glGetUniformLocation(m_program, "light.position");
	m_uniforms[LIGHT_AMBIENT] = glGetUniformLocation(m_program, "light.ambient");
	m_uniforms[LIGHT_DIFFUSE] = glGetUniformLocation(m_program, "light.diffuse");
	m_uniforms[LIGHT_SPECULAR] = glGetUniformLocation(m_program, "light.specular");

	m_uniforms[MD2_INTERPOLATION] = glGetUniformLocation(m_program, "interpolation");
}


void AnimationShader::Bind()
{
	glUseProgram(m_program);
}

AnimationShader::~AnimationShader()
{
	for (int i = 0; i < NUM_SHADERS; i++) {
		glDetachShader(m_program, m_shaders[i]);
		glDeleteShader(m_shaders[i]);
	}
	glDeleteShader(m_program);
}

void AnimationShader::Update(Light& light, AnimatedEntity& model, Camera& camera)
{	
	glUniformMatrix4fv(m_uniforms[TRANSFORM_U], 1, GL_FALSE, glm::value_ptr(model.getTransform().getModel()));
	glUniformMatrix4fv(m_uniforms[VIEW_U], 1, GL_FALSE, glm::value_ptr(camera.getView()));
	glUniformMatrix4fv(m_uniforms[PROJECTION_U], 1, GL_FALSE, glm::value_ptr(camera.getProjection()));

	glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(model.getTransform ().getModel())));
	glUniformMatrix3fv(m_uniforms[NORMAL_U], 1, GL_FALSE, glm::value_ptr(normalMatrix));

	// probably changed later: shader class should be more flexible for all the attributes...
	glUniform3f(m_uniforms[VIEWPOS], camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);

	glUniform1f(m_uniforms[MAT_SHININESS], 16.0f);

	glUniform3f(m_uniforms[LIGHT_POSITION], light.getPosition().x, light.getPosition().y, light.getPosition().z);
	glUniform3f(m_uniforms[LIGHT_AMBIENT], 0.2, 0.2, 0.2);
	glUniform3f(m_uniforms[LIGHT_DIFFUSE], 0.3f, 1.0f, 1.0f);
	glUniform3f(m_uniforms[LIGHT_SPECULAR], 1.0f, 1.0f, 1.0f);

	// update the interpolation for animating the md2 model
	glUniform1f(m_uniforms[MD2_INTERPOLATION], model.getInterpol());
}

