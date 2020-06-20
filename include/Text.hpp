#ifndef FEUD_TEXT_HPP
#define FEUD_TEXT_HPP

#include <unordered_map>

struct Font
{
    struct Character
    {
        unsigned int texture; // ID handle of the glyph texture
        glm::ivec2 size;      // Size of glyph
        glm::ivec2 bearing;   // Offset from baseline to left/top of glyph
        unsigned int advance; // Offset to advance to next glyph
    };

    std::unordered_map<char, Character> characters;
};

struct RenderTextRequest
{
    struct TextDefinition
    {
        std::string font;
        int height;
        std::string text;
        glm::vec2 position;
    };
    
    std::vector<TextDefinition> textDefintions;

    unsigned int entityId;
    unsigned int texIndex;
    sf::Color clearColor;
};

#endif // FEUD_TEXT_HPP