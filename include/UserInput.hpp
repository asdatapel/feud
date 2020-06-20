#ifndef FEUD_USERINPUT_HPP
#define FEUD_USERINPUT_HPP

struct UserInput
{
    enum struct Type
    {
        TEST_ACTION,
        BUZZ,
        SUBMIT,
        LETTER_INPUT,
        BACKSPACE,
        LEFT_CLICK,
        RIGHT_CLICK,
        SELECTION,
        READY,
    };
    
    Type t; // TODO(asad): should be named "type"
    int number = -1;
    char character;
};

#endif //FEUD_USERINPUT_HPP
