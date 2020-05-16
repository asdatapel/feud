#include "GraphicsManager.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

GraphicsManager::GraphicsManager() {}

GraphicsManager::~GraphicsManager() {}

int GraphicsManager::newShaderResource(std::string name)
{
    ShaderDefinition def = ShaderDefinition::load(name);
    ShaderResource shader;

    const char *vert = def.vert.c_str();
    GLuint vertexShader3D = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader3D, 1, &vert, NULL);
    glCompileShader(vertexShader3D);

    char log[300];
    glGetShaderInfoLog(vertexShader3D, 300, NULL, log);
    std::cout << log << '\n';

    const char *frag = def.frag.c_str();
    GLuint fragmentShader3D = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader3D, 1, &frag, NULL);
    glCompileShader(fragmentShader3D);

    glGetShaderInfoLog(fragmentShader3D, 300, NULL, log);
    std::cout << log << '\n';

    shader.handle = glCreateProgram();
    glAttachShader(shader.handle, vertexShader3D);
    glAttachShader(shader.handle, fragmentShader3D);
    glLinkProgram(shader.handle);
    glUseProgram(shader.handle);

    glDetachShader(shader.handle, vertexShader3D);
    glDetachShader(shader.handle, fragmentShader3D);
    glDeleteShader(vertexShader3D);
    glDeleteShader(fragmentShader3D);

    for (const Attribute &a : def.attributes)
    {
        shader.attributes[glGetAttribLocation(shader.handle, a.name.c_str())] = a;
    }

    for (const std::string &u : def.uniforms)
    {
        shader.uniforms[u] = glGetUniformLocation(shader.handle, u.c_str());
    }

    int next = 0;
    for (auto &i : shaderResources)
    {
        if (i.first != next)
            break;
        ++next;
    }
    shaderResources.insert({next, shader});
    return next;
}

void GraphicsManager::bindShader(int id)
{
    glUseProgram(shaderResources[id].handle);
}

unsigned int GraphicsManager::uploadTexture(const unsigned char *ptr, int sizeX, int sizeY)
{
    GLuint handle;
    glGenTextures(1, &handle);
    glBindTexture(GL_TEXTURE_2D, handle);
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA,
        sizeX, sizeY, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, ptr);

    float aniso = 4.0f;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &aniso);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, aniso);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glGenerateMipmap(GL_TEXTURE_2D);

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR)
    {
        std::cout << "uploadTexture end: " << err << std::endl;
    }

    return handle;
}

void GraphicsManager::bindTexture(int id)
{
    glBindTexture(GL_TEXTURE_2D, id);
}

int GraphicsManager::newBuffer(int shaderId)
{
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    for (auto const &attrib : shaderResources[shaderId].attributes)
    {
        glEnableVertexAttribArray(attrib.first);
        glVertexAttribPointer(attrib.first, attrib.second.length, GL_FLOAT, GL_FALSE, attrib.second.stride * sizeof(GLfloat),
                              (void *)(attrib.second.distanceToFirst * sizeof(float)));
    }

    int next = 0;
    for (auto &x : bufferResources)
    {
        if (x.first != next)
            break;
        ++next;
    }

    bufferResources.insert(std::pair<int, BufferResource>(next, {vbo, vao}));
    return next;
}

void GraphicsManager::updateBuffer(int id, float *mesh, int length)
{
    BufferResource b = bufferResources[id];
    glBindBuffer(GL_ARRAY_BUFFER, b.vbo);

    glBufferData(GL_ARRAY_BUFFER, length * sizeof(float), mesh, GL_STATIC_DRAW);
}

void GraphicsManager::renderBuffer(int id, int points)
{
    if (points != 0)
    {
        BufferResource b = bufferResources[id];
        glBindVertexArray(b.vao);

        glDrawArrays(GL_TRIANGLES, 0, points);
    }
}

void GraphicsManager::deleteBuffer(int id)
{
    BufferResource b = bufferResources[id];
    glDeleteBuffers(1, &b.vbo);
    glDeleteBuffers(1, &b.vao);
}

void GraphicsManager::uploadUniformMatrix4fv(int shaderId, std::string name, glm::mat4 mat)
{
    glUniformMatrix4fv(shaderResources[shaderId].uniforms[name], 1, GL_FALSE, glm::value_ptr(mat));
}