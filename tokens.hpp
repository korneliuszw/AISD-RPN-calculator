//
// Created by wired-mac on 09/03/2024.
//

#ifndef KALKULATOR_TOKENS_HPP
#define KALKULATOR_TOKENS_HPP

#include "functions.hpp"
enum Token {
    PARENTHESSIS_START,
    PARENTHESSIS_END,
    NEGATE,
    MULTIPLY,
    DIVIDE,
    ADD,
    SUBSTRACT,
    FUNCTION,
    ARGUMENT_SEP,
    VALUE
};
char stringifyToken(const Token &token);
int getTokenPriority(const Token &token);

struct TokenValue {
    Token token;
    union {
        Function *function = nullptr;
        int numericValue;
    };

    ~TokenValue();

    explicit TokenValue(Token token) : token(token) {};

    explicit TokenValue(Token token, Function* function) : token(token), function(function) {};
    explicit TokenValue(Token token, int value) : token(token), numericValue(value) {};

    TokenValue(const TokenValue &t);

    TokenValue &operator=(const TokenValue &r) { return *this; }
};

#endif //KALKULATOR_TOKENS_HPP
