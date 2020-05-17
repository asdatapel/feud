#ifndef FEUD_USERACTION_HPP
#define FEUD_USERACTION_HPP

struct UserAction
{
    enum struct Type
    {
        TEST_ACTION
    };

    Type t;
    int number = -1;
};

#endif //FEUD_USERACTION_HPP
