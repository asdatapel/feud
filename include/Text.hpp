#ifndef FEUD_TEXT_HPP
#define FEUD_TEXT_HPP

#include <unordered_map>

#include <ft2build.h>
#include FT_FREETYPE_H

struct Font
{
    struct Character
    {
        unsigned int textureId; // ID handle of the glyph texture
        glm::ivec2 size;        // Size of glyph
        glm::ivec2 bearing;     // Offset from baseline to left/top of glyph
        unsigned int advance;   // Offset to advance to next glyph
    };

    // mostly copied from https://learnopengl.com/In-Practice/Text-Rendering
    // TODO(asad): move this to OpenGl specific location
    void init(std::string font, int height)
    {
        FT_Library ft;
        if (FT_Init_FreeType(&ft))
            std::cout << "error init FreeType Library" << std::endl;

        FT_Face face;
        if (FT_New_Face(ft, font.c_str(), 0, &face))
            std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

        // set size to load glyphs as
        FT_Set_Pixel_Sizes(face, 0, height);

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
            Character character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                face->glyph->advance.x};

            characters.insert(std::pair<char, Character>(c, character));
        }

        FT_Done_Face(face);
        FT_Done_FreeType(ft);
    };

    void cleanup()
    {
        // TODO(asad): free everything
    }

    std::unordered_map<char, Character> characters;
};

struct RenderTextRequest {
    Font* font;
    unsigned int entityId;
    unsigned int texIndex;
    std::string text;
    glm::vec2 position;
};

#endif // FEUD_TEXT_HPP