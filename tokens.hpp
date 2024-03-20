
// Created by wired-mac on 09/03/2024.
//

#ifndef KALKULATOR_TOKENS_HPP
#define KALKULATOR_TOKENS_HPP

#include "functions.hpp"

enum Token
{
    PARENTHESSIS_START,
    PARENTHESSIS_END,
    NEGATE,
    MULTIPLY,
    DIVIDE,
    ADD,
    SUBSTRACT,
    ARGUMENT_SEP,
    VALUE,
    MIN,
    MAX,
    IF
};

inline bool isFunction(const Token& token)
{
    return token == IF || token == MAX || token == MIN;
}

Function* getTokenClass(const Token& token);


char stringifyToken(const Token& token);
int getTokenPriority(const Token& token);

struct TokenValue
{
    Token token;
    // Value if token == VALUE or number of arguments
    int numericValue;

    explicit TokenValue(Token token) : token(token)
    {
    };

    explicit TokenValue(Token token, int value) : token(token), numericValue(value)
    {
    };
};

#endif //KALKULATOR_TOKENS_HPP
