//
// Created by wired-mac on 09/03/2024.
//

#include "tokens.hpp"

char stringifyToken(const Token& token)
{
    switch (token)
    {
    case PARENTHESSIS_START:
        return '(';
    case PARENTHESSIS_END:
        return ')';
    case NEGATE:
        return 'N';
    case SUBSTRACT:
        return '-';
    case MULTIPLY:
        return '*';
    case DIVIDE:
        return '/';
    case ADD:
        return '+';
    default:
        return '\0';
    }
}

int getTokenPriority(const Token& token)
{
    if (token == PARENTHESSIS_START || token == PARENTHESSIS_END)
        return 6;
    else if (token == NEGATE || isFunction(token))
        return 5;
    else if (token == MULTIPLY || token == DIVIDE)
        return 4;
    else if (token == ADD || token == SUBSTRACT)
        return 3;
    else if (token == VALUE)
        return 2;
    return 0;
}

Function* getTokenClass(const Token& token)
{
    if (token == IF)
    {
        static auto func = new IfFunction();
        return func;
    }
    else if (token == MAX)
    {
        static auto func = new MaxFunction();
        return func;
    }
    static auto func = new MinFunction();
    return func;
}
