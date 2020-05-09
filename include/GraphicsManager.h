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

#include "Const.h"
#include "ShaderDefinition.hpp"

using namespace Const;

struct BufferResource {
	GLuint vbo;
	GLuint vao;
};

struct ShaderResource {
	GLuint handle;
	std::unordered_map<GLint, Attribute> attributes;
	std::unordered_map<std::string, GLint> uniforms;
};

struct ArrayTextureResource {
	GLuint handle;
	sf::Image image;
};

struct TextureResource {
	GLuint handle;
	sf::Image image;
	std::string name;
};

class GraphicsManager
{
public:
	GraphicsManager();
	~GraphicsManager();

	int newShaderResource(std::string name);
	void bindShader(int id);
	void deleteShader(int id);

	int getArrayTexture(std::string name);
	unsigned int uploadTexture(const unsigned char *ptr, int sizeX, int sizeY);
	int getTexture(std::string name);
	void bindArrayTexture();
	void bindTexture(int id);
	void deleteTexture(int id);

	int newBuffer(int shaderId);
	void updateBuffer(int id, float *mesh, int length);
	void renderBuffer(int id, int length);
	void deleteBuffer(int id);

	void uploadUniformMatrix4fv(int shaderId, std::string name, glm::mat4 mat);
private:
	std::map < int, BufferResource> bufferResources;
	std::map < int, ShaderResource> shaderResources;
	std::map < int, TextureResource> textureResources;
	std::map < std::string, ArrayTextureResource> arrayTextureResources;

	GLuint textureArray;
	const GLsizei textureWidth = 16;
	const GLsizei textureHeight = 16;
	const GLsizei textureCount = 256;
};

