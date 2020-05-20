#pragma once

#include <string.h>
#include <list>
#include <iostream>
#include <unordered_map>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
//#define GLEW_STATIC
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <glm/glm.hpp>

#include "ShaderDefinition.hpp"

struct BufferResource
{
	GLuint vbo;
	GLuint vao;
};

struct ShaderResource
{
	GLuint handle;
	std::unordered_map<GLint, Attribute> attributes;
	std::unordered_map<std::string, GLint> uniforms;
};

class GraphicsManager
{
public:
	GraphicsManager();
	~GraphicsManager();

	void newShaderResource(std::string name);
	void bindShader(std::string name);
	void deleteShader(std::string name);

	int getArrayTexture(std::string name);
	unsigned int uploadTexture(const unsigned char *ptr, int sizeX, int sizeY);
	int getTexture(std::string name);
	void bindTexture(int id, int index);
	void deleteTexture(int id);

	int newBuffer(std::string shader);
	void updateBuffer(int id, float *mesh, int length);
	void updateSubBuffer(int id, float *mesh, int length);
	void renderBuffer(int id, int length);
	void deleteBuffer(int id);
	

	void uploadUniformMatrix4fv(std::string shader, std::string name, glm::mat4 mat);

private:
	std::map<int, BufferResource> bufferResources;
	std::map<std::string, ShaderResource> shaderResources;
};
