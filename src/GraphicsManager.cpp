#include "GraphicsManager.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

GraphicsManager::GraphicsManager() {}

GraphicsManager::~GraphicsManager() {}

void GraphicsManager::newShaderResource(std::string name)
{
    auto it = shaderResources.find(name);
    if (it != shaderResources.end()) //already loaded?
    {
        return;
    }

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

    //TODO(asad): this should be part of the desc file
    glUniform1i(glGetUniformLocation(shader.handle, "tex1"), 0);
    glUniform1i(glGetUniformLocation(shader.handle, "tex2"), 1);

    shaderResources.insert({name, shader});

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR)
    {
        std::cout << "newShaderResource end: " << err << std::endl;
    }
}

void GraphicsManager::bindShader(std::string name)
{
    glUseProgram(shaderResources[name].handle);
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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glGenerateMipmap(GL_TEXTURE_2D);

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR)
    {
        std::cout << "uploadTexture end: " << err << std::endl;
    }

    return handle;
}

void GraphicsManager::bindTexture(int id, int index)
{
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, id);
}

int GraphicsManager::newBuffer(std::string shader)
{
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR)
    {
        std::cout << "newBuffer beg: " << err << std::endl;
    }

    for (auto const &attrib : shaderResources[shader].attributes)
    {
        glEnableVertexAttribArray(attrib.first);
        glVertexAttribPointer(attrib.first, attrib.second.length, GL_FLOAT, GL_FALSE, attrib.second.stride * sizeof(GLfloat),
                              (void *)(attrib.second.distanceToFirst * sizeof(float)));
    }

    while ((err = glGetError()) != GL_NO_ERROR)
    {
        std::cout << "newBuffer end: " << err << std::endl;
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

void GraphicsManager::updateSubBuffer(int id, float *mesh, int length)
{
    BufferResource b = bufferResources[id];
    glBindBuffer(GL_ARRAY_BUFFER, b.vbo);

    glBufferSubData(GL_ARRAY_BUFFER, 0, length, mesh);
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

void GraphicsManager::uploadUniformMatrix4fv(std::string shader, std::string name, glm::mat4 mat)
{
    glUniformMatrix4fv(shaderResources[shader].uniforms[name], 1, GL_FALSE, glm::value_ptr(mat));
}