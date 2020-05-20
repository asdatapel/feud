#ifndef FEUD_USERINPUT_HPP
#define FEUD_USERINPUT_HPP

struct UserInput
{
    enum struct Type
    {
        TEST_ACTION
    };

    Type t;
    int number = -1;
};

#endif //FEUD_USERINPUT_HPP
