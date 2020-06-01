#include "GraphicsManager.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Text.hpp"

GraphicsManager::GraphicsManager() {}

GraphicsManager::~GraphicsManager() {}

void GraphicsManager::init(std::string textShader)
{
    this->textShader = textShader;

    float textBufferData[6][4] = {
        {0.f, 1.f, 0.0f, 0.0f},
        {0.f, 0.f, 0.0f, 1.0f},
        {1.f, 0.f, 1.0f, 1.0f},

        {0.f, 1.f, 0.0f, 0.0f},
        {1.f, 0.f, 1.0f, 1.0f},
        {1.f, 1.f, 1.0f, 0.0f},
    };
    textBuffer = newBuffer(textShader);
    updateBuffer(textBuffer, (float *)textBufferData, sizeof(textBufferData));
}

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

Drawable GraphicsManager::newDrawable(Model *model, std::string shader)
{
    // copy data to a temporary buffer
    size_t vertexSize = (3 + 3 + (model->meshes[0].uvCount * 2));
    float *data = new float[model->meshes[0].vertices.size() * vertexSize];
    for (int i = 0; i < model->meshes[0].vertices.size(); ++i)
    {
        int index = i * vertexSize;
        data[index] = model->meshes[0].vertices[i].position[0];
        data[index + 1] = model->meshes[0].vertices[i].position[1];
        data[index + 2] = model->meshes[0].vertices[i].position[2];

        data[index + 3] = model->meshes[0].vertices[i].normal[0];
        data[index + 4] = model->meshes[0].vertices[i].normal[1];
        data[index + 5] = model->meshes[0].vertices[i].normal[2];

        for (int j = 0; j < model->meshes[0].uvCount; ++j)
        {
            data[index + 6 + (j * 2)] = model->meshes[0].vertices[i].uvs[j][0];
            data[index + 7 + (j * 2)] = model->meshes[0].vertices[i].uvs[j][1];
        }
    }

    int meshHandle = newBuffer(shader);
    updateBuffer(meshHandle, data, model->meshes[0].vertices.size() * vertexSize);
    delete data;

    return Drawable{meshHandle, model->meshes[0].vertices.size()};
}

Material GraphicsManager::newMaterial(std::vector<Material::Texture> textures)
{
    for (auto &t : textures)
    {
        t.handle = uploadTexture(t.data.getPixelsPtr(), t.data.getSize().x, t.data.getSize().y);
    }
    return Material{textures};
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

int GraphicsManager::newFramebuffer()
{
    GLuint fbo;
    glGenFramebuffers(1, &fbo);

    int next = 0;
    for (auto &x : framebuffers)
    {
        if (x.first != next)
            break;
        ++next;
    }
    framebuffers[next] = fbo;

    return next;
}

void GraphicsManager::bindFramebuffer(int buffer)
{
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffers[buffer]);
}

void GraphicsManager::bindTextureToFramebuffer(int buffer, unsigned int texture)
{
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
}

void GraphicsManager::resetFramebuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GraphicsManager::deleteFramebuffer(int buffer)
{
    glDeleteFramebuffers(1, &framebuffers[buffer]);
    framebuffers.erase(buffer);
}

// mostly copied from https://learnopengl.com/In-Practice/Text-Rendering
void GraphicsManager::newFont(std::string typeface, int height)
{
    Font font;

    FT_Library ft;
    if (FT_Init_FreeType(&ft))
        std::cout << "error init FreeType Library" << std::endl;

    FT_Face face;
    if (FT_New_Face(ft, typeface.c_str(), 0, &face))
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

    // set size to load glyphs as
    FT_Set_Pixel_Sizes(face, 0, height); // TODO(asad): actually calculate height somehow ?? https://stackoverflow.com/questions/60061441/freetype-correct-size-rendering

    // disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned char c = 0; c < 128; c++)
    {
        // load character glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }

        // generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer);

        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // now store character for later use
        Font::Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x,
        };

        font.characters.insert({c, character});
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    fonts[{typeface, height}] = font;
}

// mostly copied from https://learnopengl.com/In-Practice/Text-Rendering
void GraphicsManager::drawText(std::string typeface, int height, std::string text, glm::vec2 position, glm::vec2 targetSize)
{
    glm::mat4 projection = glm::ortho(0.f, targetSize.x, 0.f, targetSize.y);

    if (fonts.count({typeface, height}) == 0)
    {
        newFont(typeface, height);
    }
    Font f = fonts[{typeface, height}];

    bindShader(textShader);
    glActiveTexture(GL_TEXTURE0);

    float x = position.x;
    for (std::string::const_iterator c = text.begin(); c != text.end(); c++)
    {
        Font::Character ch = f.characters[*c];

        float xpos = x + ch.bearing.x * 1.f;
        float ypos = position.y - (ch.size.y - ch.bearing.y) * 1.f;

        float w = ch.size.x * 1.f;
        float h = ch.size.y * 1.f;
        // update VBO for each character
        float vertices[6][4] = {
            {xpos, ypos + h, 0.0f, 0.0f},
            {xpos, ypos, 0.0f, 1.0f},
            {xpos + w, ypos, 1.0f, 1.0f},

            {xpos, ypos + h, 0.0f, 0.0f},
            {xpos + w, ypos, 1.0f, 1.0f},
            {xpos + w, ypos + h, 1.0f, 0.0f}};

        glm::mat4 charProjection = glm::translate(projection, {xpos, ypos, 0});
        charProjection = glm::scale(charProjection, {w, h, 0});

        uploadUniformMatrix4fv(textShader, "Projection", charProjection);

        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.texture);
        renderBuffer(textBuffer, 6);

        x += (ch.advance >> 6) * 1.f; // bitshift by 6 to get value in pixels (2^6 = 64)
    }
}

void GraphicsManager::clear(sf::Color color)
{
    glClearColor(color.r / 255.0, color.g / 255.0, color.b / 255.0, color.a / 255.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}