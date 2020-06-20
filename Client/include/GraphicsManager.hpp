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

#include "Drawable.hpp"
#include "ShaderDefinition.hpp"
#include "Text.hpp"

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

	void init(std::string textShader);

	Drawable newDrawable(Model *model, std::string shader);
	Material newMaterial(std::vector<Material::Texture> textures);

	void newShaderResource(std::string name);
	void bindShader(std::string name);
	void deleteShader(std::string name);

	unsigned int uploadTexture(const unsigned char *ptr, int sizeX, int sizeY);
	int getTexture(std::string name);
	void bindTexture(int id, int index);
	void deleteTexture(int id);

	int newBuffer(std::string shader);
	void updateBuffer(int id, float *mesh, int length);
	void updateSubBuffer(int id, float *mesh, int length);
	void renderBuffer(int id, int length);
	void deleteBuffer(int id);

	int newFramebuffer();
	void bindFramebuffer(int buffer);
	void bindTextureToFramebuffer(int buffer, unsigned int texture);
	void resetFramebuffer(); // resets to screen buffer
	void deleteFramebuffer(const int buffer);

	void drawText(std::string typeface, int height, std::string text, glm::vec2 position, glm::vec2 targetSize);

	void uploadUniformMatrix4fv(std::string shader, std::string name, glm::mat4 mat);

	// misc
	void clear(sf::Color color);

private:
	std::map<int, BufferResource> bufferResources;
	std::map<std::string, ShaderResource> shaderResources;
	std::map<int, GLuint> framebuffers;
	std::map<std::pair<std::string, int>, Font> fonts;

	std::string textShader;
	int textBuffer;

	void newFont(std::string typeface, int height);
};
